#include "Backend.hpp"
#include <QProcess>
#include <QGuiApplication>
#include <QRegularExpression>

#include <KX11Extras>

Backend::Backend(QString homeEnv, QObject *parent)
    : QObject(parent),
    m_cpuFile("/proc/stat"),  // Inicjalizacja pliku
    m_measureCpuLoad(false),
    HOME_ENV(homeEnv)// Domyślnie pomiar jest wyłączony
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

WindowManagerDBus *Backend::getWindowManager()
{
    return &windowManagerDBus;
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


        //reset kwin state
        //windowManagerDBus.reconfigure();


        if(themeId == "Gnome"){
            qmlEngine->load(HOME_ENV + "/Bonsai/themes/gnome/Main.qml");
        }else if(themeId == "Windows XP"){
            qmlEngine->loadFromModule("XPFrontend", "Main");
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


    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.remove("LD_LIBRARY_PATH");
    env.remove("QML_IMPORT_PATH");
    env.remove("QML2_IMPORT_PATH");
    env.remove("QT_PLUGIN_PATH");
    env.remove("QTWEBENGINEPROCESS_PATH");
    env.remove("QTDIR");
    env.remove("CQT_PKG_ROOT");
    env.remove("CQT_RUN_FILE");
    env.remove("QT_QPA_PLATFORM_PLUGIN_PATH");



    env.insert("SHELL","/bin/bash");

    //env.insert("GNOME_DESKTOP_SESSION_ID","this-is-deprecated");
    //env.insert("COLORTERM","truecolor");
    //env.insert("GNOME_SHELL_SESSION_MODE","ubuntu");
    env.insert("XDG_CURRENT_DESKTOP","ubuntu:GNOME");
    env.insert("TERM","xterm-256color");
    env.insert("XDG_SESSION_CLASS","user");
    env.insert("DBUS_SESSION_BUS_ADDRESS","unix:path=/run/user/1000/bus,guid=36800c05fe8d48dcb1a58d9267b6e8d0_=/usr/bin/printenv");
    env.insert("LESSCLOSE","/usr/bin/lesspipe %s %s");
    env.insert("PATH","/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin");
    //env.insert("LOGNAME","tom");
    env.insert("XDG_SESSION_TYPE","x11");
    //env.insert("USERNAME","tom");
    env.insert("LANG","pl_PL.UTF-8");
    env.insert("XDG_SESSION_DESKTOP","ubuntu-xorg");
    env.insert("XDG_DATA_DIRS","/usr/share/ubuntu-xorg:/usr/share/gnome:/usr/local/share/:/usr/share/:/var/lib/snapd/desktop");
    env.insert("XAUTHORITY","/run/user/1000/gdm/Xauthority");
    //env.insert("SSH_AUTH_SOCK","/run/user/1000/keyring/ssh");
    //env.insert("XDG_CONFIG_DIRS","/etc/xdg/xdg-ubuntu-xorg:/etc/xdg");
    //env.insert("XDG_MENU_PREFIX","gnome-");
    //env.insert("MEMORY_PRESSURE_WRITE","c29tZSAyMDAwMDAgMjAwMDAwMAA=");
    //env.insert("HOME","/home/tom");
    //env.insert("GNOME_TERMINAL_SERVICE",":1.101");
    //env.insert("XMODIFIERS","@im=ibus");
    env.insert("DESKTOP_SESSION","ubuntu-xorg");
    //env.insert("GTK_MODULES","gail:atk-bridge");
    //env.insert("DBUS_STARTER_BUS_TYPE","session");
    //env.insert("PWD","/home/tom/Pulpit/DistributionKit");
    //env.insert("GPG_AGENT_INFO","/run/user/1000/gnupg/S.gpg-agent:0:1");
    //env.insert("SYSTEMD_EXEC_PID","2833");
    env.insert("WINDOWPATH","2");
    //env.insert("LESSOPEN","| /usr/bin/lesspipe %s");
    //env.insert("XDG_RUNTIME_DIR","/run/user/1000");
    env.insert("QT_IM_MODULE","ibus");
    //env.insert("USER","tom");
    //env.insert("DEBUGINFOD_URLS","https://debuginfod.ubuntu.com");
    //env.insert("SHLVL","1");
    //env.insert("GSM_SKIP_SSH_AGENT_WORKAROUND","true");
    //env.insert("GDMSESSION","ubuntu-xorg");
    //env.insert("GNOME_TERMINAL_SCREEN","/org/gnome/Terminal/screen/35ba9f90_9d41_4f5a_aacb_f33777e6b044");
    //env.insert("MEMORY_PRESSURE_WATCH","/sys/fs/cgroup/user.slice/user-1000.slice/user@1000.service/app.slice/app-gnome\x2dsession\x2dmanager.slice/gnome-session-manager@ubuntu.service/memory.pressure");
    //env.insert("VTE_VERSION","7800");
    //env.insert("DBUS_STARTER_ADDRESS","unix:path=/run/user/1000/bus,guid=36800c05fe8d48dcb1a58d9267b6e8d0");
    //env.insert("LS_COLORS","rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=00:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.avif=01;35:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.webp=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:*~=00;90:*#=00;90:*.bak=00;90:*.crdownload=00;90:*.dpkg-dist=00;90:*.dpkg-new=00;90:*.dpkg-old=00;90:*.dpkg-tmp=00;90:*.old=00;90:*.orig=00;90:*.part=00;90:*.rej=00;90:*.rpmnew=00;90:*.rpmorig=00;90:*.rpmsave=00;90:*.swp=00;90:*.tmp=00;90:*.ucf-dist=00;90:*.ucf-new=00;90:*.ucf-old=00;90:");
    //env.insert("SESSION_MANAGER","local/Clean-Ubuntu-24-10:@/tmp/.ICE-unix/2833,unix/Clean-Ubuntu-24-10:/tmp/.ICE-unix/2833");
    env.insert("QT_ACCESSIBILITY","1");


    qDebug() << __PRETTY_FUNCTION__ << " remove system enviroment variables";
    qDebug() << __PRETTY_FUNCTION__ << " ProcessEnviroment = " << env.toStringList();

    process.setProcessEnvironment(env);
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
