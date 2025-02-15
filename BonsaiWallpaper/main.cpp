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

    QProcess process;
    process.start("kwin");
    process.waitForStarted();

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

