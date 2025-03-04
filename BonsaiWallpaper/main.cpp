#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QProcess>
#include <QObject>
#include <QQmlContext>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QStandardPaths>
#include <QDir>
#include <QtQuick3D/qquick3d.h>
#include <QWindow>
#include <QtWebEngineQuick>

#include "../Bonsai/Backend/Backend.hpp"
#include "../Bonsai/Backend/AppsListModel.hpp"
#include "../Bonsai/Backend/BackendAppsIconsProvider.hpp"
#include "../Bonsai/Backend/Model.hpp"
#include "../Bonsai/Backend/AudioBackend.hpp"
#include "../Bonsai/Backend/FavoriteAppsListModel.hpp"

#include <QtQml/QQmlExtensionPlugin>
Q_IMPORT_QML_PLUGIN(XPFrontendPlugin)

int main(int argc, char *argv[])
{

    QString homePath = qgetenv("HOME");
    QString modulePath = homePath + "/Bonsai/themes/gnome";

    QGuiApplication::setApplicationName("bonsai");
    QGuiApplication::setOrganizationName("Bonsai");



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
    env.insert("XDG_CURRENT_DESKTOP","ubuntu:GNOME");
    env.insert("TERM","xterm-256color");
    env.insert("XDG_SESSION_CLASS","user");
    env.insert("DBUS_SESSION_BUS_ADDRESS","unix:path=/run/user/1000/bus,guid=36800c05fe8d48dcb1a58d9267b6e8d0_=/usr/bin/printenv");
    env.insert("LESSCLOSE","/usr/bin/lesspipe %s %s");
    env.insert("PATH","/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin");
    env.insert("XDG_SESSION_TYPE","x11");
    env.insert("LANG","pl_PL.UTF-8");
    env.insert("XDG_SESSION_DESKTOP","ubuntu-xorg");
    env.insert("XDG_DATA_DIRS","/usr/share/ubuntu-xorg:/usr/share/gnome:/usr/local/share/:/usr/share/:/var/lib/snapd/desktop");
    env.insert("XAUTHORITY","/run/user/1000/gdm/Xauthority");
    env.insert("DESKTOP_SESSION","ubuntu-xorg");
    env.insert("WINDOWPATH","2");
    env.insert("QT_IM_MODULE","ibus");
    env.insert("QT_ACCESSIBILITY","1");



    QProcess process;
    process.setProcessEnvironment(env);
    process.start("kwin");
    process.waitForStarted(5000);

    QtWebEngineQuick::initialize();
    QGuiApplication app(argc, argv);

    // QStringList paths;
    // paths << "/home/rafal/Bonsai/themes/windows_xp/icons";
    // QIcon::setThemeSearchPaths(paths);


    // Odczyt zmiennej środowiskowej DISPLAY
    QByteArray displayEnv = qgetenv("DISPLAY");
    if (displayEnv.isEmpty()) {
        qWarning() << "Zmienna środowiskowa DISPLAY nie jest ustawiona!";
    } else {
        qDebug() << "Zmienna DISPLAY ma wartość:" << displayEnv;
    }

    QSurfaceFormat::setDefaultFormat(QQuick3D::idealSurfaceFormat());

    QQmlApplicationEngine engine;
    engine.addImportPath(modulePath);



    // //Try load
    // QString homePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    // QString qmlFilePath = QDir(homePath).filePath("Main.qml");  // np. ~/username/myqmlfile.qml
    // // Sprawdzenie, czy plik istnieje
    // if (!QFile::exists(qmlFilePath)) {
    //     qWarning() << "Plik QML nie istnieje:" << qmlFilePath;
    //     return -1;
    // }
    // // Ładowanie pliku QML
    // engine.load(QUrl::fromLocalFile(qmlFilePath));
    // // Sprawdzenie, czy plik się załadował poprawnie
    // if (engine.rootObjects().isEmpty())
    //     return -1;

    ApplicationModel appsListModel;
    FilterProxyModel filterProxyModel;
    filterProxyModel.setSourceModel(&appsListModel);


    FavoriteAppsProxyModel favoriteAppsModel;
    favoriteAppsModel.setSourceModel(&appsListModel);


    Backend backend(homePath);
    backend.setQmlEngine(&engine);

    engine.addImageProvider(QLatin1String("backend_app_icon"), new BackendAppsIconsProvider);

    engine.rootContext()->setContextProperty("HOME", homePath);
    //Apps List models
    engine.rootContext()->setContextProperty("appsListModel", &filterProxyModel);
    engine.rootContext()->setContextProperty("favoriteAppsModel", &favoriteAppsModel);
    engine.rootContext()->setContextProperty("backend", &backend);

    //Taskbar backend
    MyListModel model;
    engine.rootContext()->setContextProperty("backend_taskbarModel", &model);
    engine.addImageProvider(QLatin1String("backendTaskbarIcons"), new TaskbarIconsProvider);

    //Audio backend
    AudioBackend audioBackend;
    engine.rootContext()->setContextProperty("audioBackend", &audioBackend);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() {
                         QCoreApplication::exit(-1);
    },
        Qt::QueuedConnection);
        //engine.loadFromModule("GnomeModule", "Main");
        //engine.load("/home/rafal/Bonsai/themes/gnome/Main.qml");
        engine.loadFromModule("XPFrontend", "Main");
        //engine.loadFromModule("KikoWallpaper", "Main");


    // Upewnij się, że główny obiekt QML jest załadowany
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

#include "main.moc"

