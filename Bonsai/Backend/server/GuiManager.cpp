#include "GuiManager.hpp"

#include <QDBusConnectionInterface>
#include <QDBusServiceWatcher>

#include "../helper/Process.hpp"

static bool startKwinAndWaitForReady(QProcess &process, int timeoutMs = 15000);

GuiManager::GuiManager(QObject *parent, QGuiApplication *app, int swapIntervalOption)
    : QObject(parent)
    , m_x11WindowManagerService(this)
    , m_qmlGui(this, app, swapIntervalOption)
{
    HOME_ENV = qgetenv("HOME");
    //START KWIN
    startKwinAndWaitForReady(m_kwin);

    //---- Start Gui ----
    m_qmlGui.initQmlEngine();
    m_qmlGui.loadFromModule("XPFrontend", "Main");
    //m_gui.load(HOME_ENV + "/Bonsai/themes/gnome/Main.qml");
}

GuiManager::~GuiManager() {}

void GuiManager::setActiveFrontend(const QString &themeId)
{
    qDebug() << __PRETTY_FUNCTION__ << "Gui manager: main switch frontend function";

    m_qmlGui.deleteQmlEngineRootObjects();

    //reset kwin state
    //m_x11WindowManagerService.reconfigure();

    if (themeId == "862b771db54b6d6764f8d72ece0e31e994bf0058") {
        m_qmlGui.load(HOME_ENV + "/Bonsai/themes/gnome/Main.qml");
    } else if (themeId == "351c97c34271d2a3ac6c180b951c920528de19d8") { //XP Luna
        m_qmlGui.loadFromModule("XPFrontend", "Main");
    }

    qDebug() << __PRETTY_FUNCTION__ << " emit frontendChanged()";
    emit frontendChanged();
}

void GuiManager::uninit()
{
    m_qmlGui.deleteQmlEngineRootObjects();

    if (m_kwin.state() != QProcess::NotRunning) {
        m_kwin.terminate();
        m_kwin.waitForFinished();
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
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    filterProcessEnvironment(env, "/opt/Bonsai/DistributionKit_2"); // Odkomentuj jeśli potrzebne
    process.setProcessEnvironment(env);

    // Start procesu
    process.start("kwin_x11"); // lub "kwin_wayland" w zależności od sesji

    if (!process.waitForStarted(3000)) {
        qCritical() << "Nie udało się uruchomić procesu kwin! Błąd:" << process.errorString();
        return false;
    }

    qDebug() << "Proces kwin uruchomiony. Czekam na rejestrację usługi D-Bus 'org.kde.KWin'...";

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
        qInfo().nospace() << "KWin jest gotowy (usługa D-Bus zarejestrowana). Uruchomienie zajęło: "
                          << elapsedMs << " ms.";

        return true;
    } else {
        // Timer się skończył
        qCritical() << "Timeout! KWin nie zarejestrował swojej usługi D-Bus w ciągu" << timeoutMs
                    << "ms.";
        process.terminate(); // Zakończ proces, który prawdopodobnie zawisł
        process.waitForFinished(1000);
        return false;
    }
}
