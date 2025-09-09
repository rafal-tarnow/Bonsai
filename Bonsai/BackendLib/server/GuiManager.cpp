#include "GuiManager.hpp"

#include <QDBusConnectionInterface>
#include <QDBusServiceWatcher>

#include "../helper/Process.hpp"

static bool startKwinAndWaitForReady(QProcess &process, int timeoutMs = 15000);

GuiManager::GuiManager(QObject *parent, QGuiApplication *app, int swapIntervalOption)
    : QObject(parent)
    , m_x11WindowManagerService(this)
    , m_qmlGui(this, app, swapIntervalOption)
    , changeFrontendStateMachine(this)
{
        HOME_ENV = qgetenv("HOME");

    connect(&m_kwin, &QProcess::readyReadStandardOutput, [&]() {
        qDebug() << "[INFO KWIN] " << m_kwin.readAllStandardOutput();
    });

    connect(&m_kwin, &QProcess::readyReadStandardError, [&]() {
        qDebug() << "[ERROR KWIN] " << m_kwin.readAllStandardError();
    });
}

GuiManager::~GuiManager()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void GuiManager::startGui(const FrontendInfo & frontend)
{




    //START KWIN
    startKwinAndWaitForReady(m_kwin);

    //---- Start Gui ----
    m_qmlGui.initQmlEngine();

    m_currentFrontend = frontend;
    loadFrontend();

    connect(&m_x11WindowManagerService,
            &WindowManagerX11Service::reconfigureFinished,
            this,
            &GuiManager::handleKwinReconfigured);

    changeFrontendStateMachine.start();

    changeFrontendStateMachine.connectToState("DeletingQmlObjects", [=](bool active) {
        if (active) {
            qDebug() << "Enter state: " << "DeletingQmlObjects";
            m_qmlGui.deleteQmlEngineRootObjects();
            changeFrontendStateMachine.submitEvent("qmlObjectsDeleted");
        } else {
            qDebug() << "Exit state: " << "DeletingQmlObjects";
        }
    });

    changeFrontendStateMachine.connectToState("ReconfiguringWindowManager", [=](bool active) {
        if (active) {
            qDebug() << "Enter state: " << "ReconfiguringWindowManager";
            m_x11WindowManagerService.reconfigure();
        } else {
            qDebug() << "Exit state: " << "ReconfiguringWindowManager";
        }
    });

    changeFrontendStateMachine.connectToState("EmittingFrontendChanged", [=](bool active) {
        if (active) {
            qDebug() << "Enter state: " << "EmittingFrontendChanged";

            qDebug() << "Server 9 " << __PRETTY_FUNCTION__ << " emit frontendChanged()";
            emit frontendChanged(m_currentFrontend.id);

            changeFrontendStateMachine.submitEvent("frontendChangedEmitted");
        } else {
            qDebug() << "Exit state: " << "EmittingFrontendChanged";
        }
    });

    changeFrontendStateMachine.connectToState("LoadingFrontend", [=](bool active) {
        if (active) {
            qDebug() << "Enter state: " << "LoadingFrontend";

            loadFrontend();

            changeFrontendStateMachine.submitEvent("frontendLoaded");
        } else {
            qDebug() << "Exit state: " << "LoadingFrontend";
        }
    });
}

void GuiManager::tryLoadFrontend(const FrontendInfo &frontend)
{
    m_currentFrontend = frontend;
    changeFrontendStateMachine.submitEvent("startFrontendChange");
}

void GuiManager::uninit()
{
    m_qmlGui.deleteQmlEngineRootObjects();

    if (m_kwin.state() != QProcess::NotRunning) {
        m_kwin.terminate();
        m_kwin.waitForFinished();
    }
}

void GuiManager::handleKwinReconfigured()
{
    qDebug() << __PRETTY_FUNCTION__ << changeFrontendStateMachine.activeStateNames();
    if (changeFrontendStateMachine.isActive("ReconfiguringWindowManager")) {
        qDebug() << "changeFrontendStateMachine.submitEvent(windowManagerReconfigured);";
        changeFrontendStateMachine.submitEvent("windowManagerReconfigured");
    }
}

void GuiManager::loadFrontend()
{
    if(m_currentFrontend.qmlFilePath != ""){
        m_qmlGui.load(m_currentFrontend.qmlFilePath);
    }else if (m_currentFrontend.qmlUri != "") {
        m_qmlGui.loadFromModule(m_currentFrontend.qmlUri, m_currentFrontend.qmlTypeName);
    }
}

bool startKwinAndWaitForReady(QProcess &process, int timeoutMs)
{
    // Sprawdź, czy KWin już nie działa
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if (sessionBus.interface()->isServiceRegistered("org.kde.KWin")) {
        qDebug() << "KWin już działa, nie uruchamiam nowego procesu.";
        return true;
    }

    qDebug() << "Uruchamianie procesu kwin...";

    // NOWOŚĆ: Utwórz i uruchom stoper do mierzenia czasu
    QElapsedTimer startupTimer;
    startupTimer.start();

    // Ustawienie środowiska tak jak poprzednio
    // Dlaczego filtrujemy zmienne środowiskowe, założmy ze mamy bonsai skompilowane z innymi bibliotekami niż kwin w docelowym systemi, jezeli w zmiennych bedzie
    // LD_LIBRARY_PATH do sciezki z bibliotekami bonsai to kwin bedzie linkowal do bibliotek bonsai zamiast do bibliotek systemowych
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    QString homePath = qgetenv("HOME");
    // filterProcessEnvironment(env,
    //                          homePath
    //                              + "/Bonsai_install/Bonsai_1.0.0/lib"); // Odkomentuj jeśli potrzebne

    filterProcessEnvironment(env, "/opt/Bonsai/Bonsai_1.0.0/lib");
    process.setProcessEnvironment(env);

    // Start procesu
    process.start("kwin_x11"); // lub "kwin_wayland" w zależności od sesji

    if (!process.waitForStarted(5000)) {
        qDebug() << "Nie udało się uruchomić procesu kwin! Błąd:" << process.errorString();
        return false;
    }

    qDebug() << "[STARTUP INFO] Proces kwin uruchomiony. Czekam na rejestrację usługi D-Bus "
                "'org.kde.KWin'...";

    // Użyj QDBusServiceWatcher do czekania na gotowość
    QDBusServiceWatcher watcher("org.kde.KWin",
                                sessionBus,
                                QDBusServiceWatcher::WatchForRegistration);

    // Utwórz pętlę zdarzeń, aby czekać na sygnał bez blokowania wszystkiego
    QEventLoop loop;

    // Połącz sygnał pojawienia się usługi z wyjściem z pętli zdarzeń
    QObject::connect(&watcher, &QDBusServiceWatcher::serviceRegistered, &loop, &QEventLoop::quit);

    // Ustaw timeout, aby nie czekać w nieskończoność
    QTimer timeoutTimer; // Zmiana nazwy dla czytelności
    timeoutTimer.setSingleShot(true);
    QObject::connect(&timeoutTimer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timeoutTimer.start(timeoutMs);

    // Uruchom pętlę zdarzeń. Wyjdzie z niej, gdy usługa się zarejestruje lub gdy minie timeout.
    loop.exec();

    // Sprawdź, dlaczego wyszliśmy z pętli
    if (timeoutTimer.isActive()) {
        // Timer jest wciąż aktywny, co oznacza, że to sygnał z watchera zakończył pętlę
        timeoutTimer.stop();

        // NOWOŚĆ: Pobierz i wyświetl czas, jaki upłynął
        qint64 elapsedMs = startupTimer.elapsed();
        qDebug().nospace() << "[STARTUP INFO] KWin jest gotowy (usługa D-Bus zarejestrowana). "
                              "Uruchomienie zajęło: "
                           << elapsedMs << " ms.";

        return true;
    } else {
        // Timer się skończył
        qDebug() << "[STARTUP ERROR] Timeout! KWin nie zarejestrował swojej usługi D-Bus w ciągu"
                 << timeoutMs << "ms.";
        process.terminate(); // Zakończ proces, który prawdopodobnie zawisł
        process.waitForFinished(1000);
        return false;
    }
}
