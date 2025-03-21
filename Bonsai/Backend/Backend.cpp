#include "Backend.hpp"
#include <QProcess>
#include <QGuiApplication>
#include <QRegularExpression>

#include <KX11Extras>

Backend::Backend(QObject *parent)
    : QObject(parent),
    m_cpuFile("/proc/stat"),  // Inicjalizacja pliku
    m_measureCpuLoad(false)   // Domyślnie pomiar jest wyłączony
{
    m_platformName = QGuiApplication::platformName();
    if (m_platformName == "xcb") {
        m_platformName = "X11";
    }
    emit platformNameChanged();

    //cpu load init
    connect(&m_timer, &QTimer::timeout, this, &Backend::updateCpuLoad);
}

Backend::~Backend() {
    if (m_cpuFile.isOpen()) {
        m_cpuFile.close();  // Zamknięcie pliku w destruktorze
    }
}

void Backend::setQmlEngine(QQmlApplicationEngine *engine)
{
    qmlEngine = engine;
}

ThemesModel *Backend::getThemesModel()
{
    return &themesModel;
}

void Backend::setActiveFrontend(const QString &themeId)
{
    themesModel.setActiveFrontend(themeId);
    if(qmlEngine){

        //close all root QQuickWindows, if we dont close all windows, when we switch frontend, thhey sometimes stay some windows, for example some side panel windows
        for (QObject* rootObject : qmlEngine->rootObjects()) {
            if (QQuickWindow* window = qobject_cast<QQuickWindow*>(rootObject)) {
                window->close();
                window->deleteLater();
            }
        }

        qmlEngine->clearComponentCache();
        if(themeId == "Gnome"){
            qmlEngine->load("/home/rafal/Bonsai/themes/gnome/Main.qml");
        }else if(themeId == "Windows XP"){
            qmlEngine->loadFromModule("Bonsai.Wallpaper", "Main");
        }
    }
}

void Backend::logout() {
    QCoreApplication::exit(0);
}

void Backend::reboot() {
    QDBusMessage message = QDBusMessage::createMethodCall(
        "org.freedesktop.login1",
        "/org/freedesktop/login1",
        "org.freedesktop.login1.Manager",
        "Reboot"
        );
    message << true;
    QDBusMessage reply = QDBusConnection::systemBus().call(message);
    if (reply.type() == QDBusMessage::ErrorMessage) {
        qWarning() << "Failed to reboot:" << reply.errorMessage();
    } else {
        qDebug() << "Reboot command sent successfully.";
    }
}

void Backend::poweroff() {
    QDBusMessage message = QDBusMessage::createMethodCall(
        "org.freedesktop.login1",
        "/org/freedesktop/login1",
        "org.freedesktop.login1.Manager",
        "PowerOff"
        );
    message << true;
    QDBusMessage reply = QDBusConnection::systemBus().call(message);
    if (reply.type() == QDBusMessage::ErrorMessage) {
        qWarning() << "Failed to power off:" << reply.errorMessage();
    } else {
        qDebug() << "Power off command sent successfully.";
    }
}

void Backend::activateWindow(WId win)
{
    KX11Extras::activateWindow(win);
}

void Backend::minimalizeAllWindows()
{
    if (m_platformName == "X11") {
        const QList<WId> windows = KX11Extras::windows();
        for (const WId &windowId : windows) {
            KX11Extras::minimizeWindow(windowId);
        }
    } else {
        qWarning() << "Minimize all windows is not supported on this platform.";
    }
}

void Backend::startApplication(const QString &id)
{

}

void Backend::startProcess(const QString &proc) {
    QProcess::startDetached(proc);
}

void Backend::runCommand(const QString &cmd) {
    QProcess process;
    QStringList parts = cmd.split(' ', Qt::SkipEmptyParts);
    QString program = parts.takeFirst();
    QStringList filteredArgs;
    for (const QString &arg : parts) {
        if (!arg.startsWith('%')) {
            filteredArgs << arg;
        }
    }
    process.startDetached(program, filteredArgs);
}

void Backend::reservePanelLeftArea(QQuickWindow * window, int x, int y, int width, int height)
{
    //We call this method in the next iteration of the event loop because, if we run it directly from
    //QML while the QML file is still loading, the window ID has not yet been initialized. We need to
    //wait until the window is created before we can set the area occupied by the panel.
    QMetaObject::invokeMethod(this, [=]() {
        if (window) {
            reservePanelLeftArea(window->winId(), y, y + height, width);
        } else {
            qDebug() << "window id is empty";
        }
    }, Qt::QueuedConnection);

}

void Backend::reservePanelLeftArea(WId windowId, int x, int y, int width, int height)
{
    KX11Extras::setExtendedStrut(
        windowId,
        width,  // Left width
        y,      // Left start
        y + height,  // Left end
        0, 0, 0,     // Right strut (unused)
        0, 0, 0,     // Top strut (unused)
        0, 0, 0      // Bottom strut (unused)
        );
}

void Backend::reservePanelLeftArea(WId windowId, int start_y, int stop_y, int width)
{
    KX11Extras::setExtendedStrut(
        windowId,
        width,       // Left width
        start_y,     // Left start
        stop_y,      // Left end
        0, 0, 0,     // Right strut (unused)
        0, 0, 0,     // Top strut (unused)
        0, 0, 0      // Bottom strut (unused)
        );
}

void Backend::reservePanelTopArea(QQuickWindow *window, int x, int y, int width, int height)
{
    //We call this method in the next iteration of the event loop because, if we run it directly from
    //QML while the QML file is still loading, the window ID has not yet been initialized. We need to
    //wait until the window is created before we can set the area occupied by the panel.

    QMetaObject::invokeMethod(this, [=]() {
        if (window) {
            reservePanelTopArea(window->winId(), x, x + width, height);
        } else {
            qDebug() << "window id is empty";
        }
    }, Qt::QueuedConnection);
}

void Backend::reservePanelTopArea(WId windowId, int x, int y, int width, int height)
{
    KX11Extras::setExtendedStrut(
        windowId,
        0, 0, 0,            // Left strut (unused)
        0, 0, 0,            // Right strut (unused)
        height,             // Top width
        x,                  // Top start
        x + width,          // Top end
        0, 0, 0             // Bottom strut (unused)
        );
}

void Backend::reservePanelTopArea(WId windowId, int start_x, int stop_x, int height)
{
    KX11Extras::setExtendedStrut(
        windowId,
        0, 0, 0,            // Left strut (unused)
        0, 0, 0,            // Right strut (unused)
        height,             // Top width
        start_x,            // Top start
        stop_x,             // Top end
        0, 0, 0             // Bottom strut (unused)
        );
}

void Backend::reservePanelBottomArea(QQuickWindow *window, int x, int y, int width, int height)
{
    //We call this method in the next iteration of the event loop because, if we run it directly from
    //QML while the QML file is still loading, the window ID has not yet been initialized. We need to
    //wait until the window is created before we can set the area occupied by the panel.

    QMetaObject::invokeMethod(this, [=]() {
        if (window) {
            reservePanelBottomArea(window->winId(), x, width, height);
        } else {
            qDebug() << "window id is empty";
        }
    }, Qt::QueuedConnection);
}

void Backend::reservePanelBottomArea(WId windowId, int x, int y, int width, int height)
{
    KX11Extras::setExtendedStrut(
        windowId,
        0, 0, 0,            // lewy strut (nieużywany)
        0, 0, 0,            // prawy strut (nieużywany)
        0, 0, 0,            // górny strut (nieużywany)
        height,         // bottom width
        x,              //bottom start
        x + width       // bottom end
        );
}

void Backend::reservePanelBottomArea(WId windowId, int start_x, int stop_x, int height)
{
    KX11Extras::setExtendedStrut(
        windowId,
        0, 0, 0,            // lewy strut (nieużywany)
        0, 0, 0,            // prawy strut (nieużywany)
        0, 0, 0,            // górny strut (nieużywany)
        height,             // bottom width
        start_x,            //bottom start
        stop_x              // bottom end
        );
}

void Backend::setX11WindowTypeAsDesktop(QQuickWindow *window)
{
    qDebug() << __PRETTY_FUNCTION__;
    if(window){
        KX11Extras::setType(window->winId(), NET::WindowType::Desktop);
    }
}

void Backend::setX11WindowTypeAsDock(QQuickWindow *window)
{
    qDebug() << __PRETTY_FUNCTION__;
    if(window){
        KX11Extras::setType(window->winId(), NET::WindowType::Dock);
    }
}

QString Backend::platformName() const {
    return m_platformName;
}

QString Backend::qtVersion() const
{
    return QString(qVersion());
}

float Backend::cpuLoad() const { return m_cpuLoad; }

bool Backend::measureCpuLoad() const {
    return m_measureCpuLoad;
}

void Backend::setMeasureCpuLoad(bool enable) {
    if (m_measureCpuLoad == enable) return;  // Sprawdzenie, czy stan się zmienił

    m_measureCpuLoad = enable;
    emit measureCpuLoadChanged();

    if (m_measureCpuLoad) {
        // Włączenie pomiaru CPU
        if (!m_cpuFile.isOpen()) {
            if (!m_cpuFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qWarning() << "Nie można otworzyć pliku /proc/stat";
                return;
            }
        }
        m_timer.start(1000);  // Uruchomienie timera
    } else {
        // Wyłączenie pomiaru CPU
        m_timer.stop();  // Zatrzymanie timera
        if (m_cpuFile.isOpen()) {
            m_cpuFile.close();  // Zamknięcie pliku
        }
    }
}

void Backend::updateCpuLoad() {
    if (!m_cpuFile.isOpen()) {
        qWarning() << "Plik /proc/stat nie jest otwarty";
        return;
    }

    // Ustawienie wskaźnika pliku na początek, aby odczytać nowe dane
    m_cpuFile.seek(0);

    QTextStream in(&m_cpuFile);
    QString line = in.readLine();

    QStringList cpuData = line.split(QRegularExpression("\\s+"));
    if (cpuData.size() < 8) return;

    long long user = cpuData[1].toLongLong();
    long long nice = cpuData[2].toLongLong();
    long long system = cpuData[3].toLongLong();
    long long idle = cpuData[4].toLongLong();
    long long iowait = cpuData[5].toLongLong();
    long long irq = cpuData[6].toLongLong();
    long long softirq = cpuData[7].toLongLong();

    long long total = user + nice + system + idle + iowait + irq + softirq;
    long long totalIdle = idle + iowait;

    if (prevTotal != 0 && prevIdle != 0) {
        long long totalDiff = total - prevTotal;
        long long idleDiff = totalIdle - prevIdle;

        m_cpuLoad = (1.0f - (float(idleDiff) / float(totalDiff))) * 100.0f;
        emit cpuLoadChanged();
    }

    prevTotal = total;
    prevIdle = totalIdle;
}
