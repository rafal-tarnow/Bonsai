#include <csignal>
#include <memory>

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

#include <Backend.hpp>
#include <BSessionManager.hpp>
#include <server/Server.hpp>
#include <AppsListModel.hpp>
#include <BackendAppsIconsProvider.hpp>
#include <Model.hpp>
#include <AudioBackend.hpp>
#include <FavoriteAppsListModel.hpp>
#include <helper/Process.hpp>

#include <QtQml/QQmlExtensionPlugin>
Q_IMPORT_QML_PLUGIN(XPFrontendPlugin)

void getOptions(const QCoreApplication &app, QString &modeOption, QString &sourceOption, int &x, int &y, int &width, int &height,  int &swapInterval);
static void signalHandler(int signal) {
    qDebug() << "Otrzymano sygnał:" << signal << ". Zamykam aplikację...";
    QCoreApplication::quit();
}
void startKwin(QProcess &process);
void setWindowGeometry(QQmlApplicationEngine &engine, int x, int y, int width, int height);

int main(int argc, char *argv[])
{
    // Rejestracja obsługi sygnałów systemowych
    std::signal(SIGTERM, signalHandler); // Sygnał zakończenia (np. z Qt Creatora lub kill)
    std::signal(SIGINT, signalHandler);  // Ctrl+C w terminalu


    //READ ENVIROMENT VARIABLES
    QString homePath = qgetenv("HOME");
    QString modulePath = homePath + "/Bonsai/themes/gnome";

    //START KWIN
    QProcess kwin;
    startKwin(kwin);



    //INIT APPLICATION
    QGuiApplication::setApplicationName("bonsai");
    QGuiApplication::setOrganizationName("Bonsai");
    QtWebEngineQuick::initialize();
    QGuiApplication app(argc, argv);

    //INIT APPLICATION OPTIONS
    QString modeOption, sourceOption;
    int xOption, yOption, widthOption, heightOption, swapInterwalOption;
    getOptions(app, modeOption, sourceOption, xOption, yOption, widthOption, heightOption, swapInterwalOption);

    if(false){//modeOption == "client"){
        qDebug() << "ŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻ";
        QUrl sourceUrl(sourceOption);
        if(!sourceUrl.isValid())
            return -1;



        //INIT SURFACE
        QSurfaceFormat surfaceFormat = QQuick3D::idealSurfaceFormat();
        //QSurfaceFormat surfaceFormat = QSurfaceFormat::defaultFormat();
        surfaceFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
        surfaceFormat.setSwapInterval(swapInterwalOption);
        QSurfaceFormat::setDefaultFormat(surfaceFormat);


        QQmlApplicationEngine engine;
        engine.addImportPath(modulePath);


        QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app,
                         []() {
                             QCoreApplication::exit(-1);
                         },
                         Qt::QueuedConnection);
        engine.load(sourceOption);

        //Make sure the main QML object is loaded.
        if (engine.rootObjects().isEmpty())
            return -1;

        return app.exec();


        // QQuickView view;
        // //view.loadFromModule("XPFrontend", "Clock3D");
        // //view.setSource(QUrl("qrc:///Clock3D.qml"));
        // //view.setSource(QUrl("qrc:Clock3D.qml"));
        // view.setSource(sourceUrl);
        // view.setGeometry(QRect(xOption, yOption, widthOption, heightOption));
        // view.show();
        // return app.exec();

    }



    //INIT SURFACE
    QSurfaceFormat surfaceFormat = QQuick3D::idealSurfaceFormat();
    //QSurfaceFormat surfaceFormat = QSurfaceFormat::defaultFormat();
    surfaceFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    surfaceFormat.setSwapInterval(swapInterwalOption);
    QSurfaceFormat::setDefaultFormat(surfaceFormat);



    QQmlApplicationEngine engine;
    engine.addImportPath(modulePath);

    std::unique_ptr<Server> serverPtr;
    if(modeOption == "server"){
        serverPtr = std::make_unique<Server>(&engine);
    }


    //INIT QML ENGINE ICON PROVIDERS
    BackendAppsIconsProvider appsIconProvider;
    engine.addImageProvider(QLatin1String("backend_app_icon"), &appsIconProvider);
    engine.addImageProvider(QLatin1String("backendTaskbarIcons"), new TaskbarIconsProvider);

    //INIT QML ENGINE CONTEXT PROPERTIES
    ApplicationModel appsListModel;
    FilterProxyModel filterProxyModel;
    filterProxyModel.setSourceModel(&appsListModel);

    FavoriteAppsProxyModel favoriteAppsModel;
    favoriteAppsModel.setSourceModel(&appsListModel);

    Backend backend(homePath);
    backend.setQmlEngine(&engine);


    engine.rootContext()->setContextProperty("HOME", homePath);
    //Apps List models
    engine.rootContext()->setContextProperty("appsListModel", &filterProxyModel);
    engine.rootContext()->setContextProperty("favoriteAppsModel", &favoriteAppsModel);
    engine.rootContext()->setContextProperty("backend", &backend);
    //Audio backend
    AudioBackend audioBackend;
    engine.rootContext()->setContextProperty("audioBackend", &audioBackend);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app,
                     []() {
                         QCoreApplication::exit(-1);
                     },
                     Qt::QueuedConnection);
    //engine.load("/home/rafal/Bonsai/themes/gnome/Main.qml");
    if(modeOption == "server"){
        //SERVER
        engine.loadFromModule("XPFrontend", "Main");
    }else{
        //CLIENT
        engine.load(sourceOption);
        setWindowGeometry(engine, xOption, yOption, widthOption, heightOption);
    }

    //Make sure the main QML object is loaded.
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

void getOptions(const QCoreApplication &app, QString &modeOption, QString &sourceOption, int &x, int &y, int &width, int &height, int &swapInterval)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Command-line options parser for the application");
    parser.addHelpOption();
    parser.addVersionOption();

    // Define command-line options
    QCommandLineOption modeOpt("mode", "Set application mode (server/client)", "value", "server");
    QCommandLineOption sourceOpt("source", "Set QML source URL (e.g., qrc:///Clock3D.qml)", "value");
    QCommandLineOption xOpt("x", "Set window X position", "x", "100");
    QCommandLineOption yOpt("y", "Set window Y position", "y", "100");
    QCommandLineOption widthOpt("width", "Set window width", "width", "800");
    QCommandLineOption heightOpt("height", "Set window height", "height", "600");
    QCommandLineOption swapIntervalOpt("swap-interval", "Set swap interval", "value", "1"); // Nowa opcja

    // Add options to parser
    parser.addOptions({modeOpt, sourceOpt, xOpt, yOpt, widthOpt, heightOpt, swapIntervalOpt});

    // Process arguments
    parser.process(app);

    // Set default values
    modeOption = parser.value(modeOpt);
    sourceOption = parser.value(sourceOpt);
    x = 100;
    y = 100;
    width = 800;
    height = 600;
    swapInterval = 0; // Wartość domyślna dla swap-interval

    // Parse mode option
    if (parser.isSet(modeOpt)) {
        QString value = parser.value(modeOpt).toLower();
        if (value == "server" || value == "client") {
            modeOption = value;
        } else {
            qWarning() << "Invalid --mode value:" << value << "(expected: server or client)";
        }
    }

    // Parse source option
    if (parser.isSet(sourceOpt)) {
        qDebug() << "Source set to:" << sourceOption;
    }

    // Parse geometry options with validation
    auto parseIntOption = [&parser](const QCommandLineOption &opt, int &target, const char *name) {
        if (parser.isSet(opt)) {
            bool ok;
            int value = parser.value(opt).toInt(&ok);
            if (ok && value >= 0) {
                target = value;
            } else {
                qWarning() << "Invalid" << name << "value:" << parser.value(opt) << "(expected: non-negative integer)";
            }
        }
    };

    parseIntOption(xOpt, x, "--x");
    parseIntOption(yOpt, y, "--y");
    parseIntOption(widthOpt, width, "--width");
    parseIntOption(heightOpt, height, "--height");
    parseIntOption(swapIntervalOpt, swapInterval, "--swap-interval"); // Parsowanie swap-interval
}

void startKwin(QProcess &process){
    //START KWIN
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    filterProcessEnvironment(env, "/opt/Bonsai/DistributionKit_2");
    process.setProcessEnvironment(env);
    process.start("kwin");
    process.waitForStarted(5000);
}

void setWindowGeometry(QQmlApplicationEngine &engine, int x, int y, int width, int height)
{
    if (!engine.rootObjects().isEmpty()) {
        QObject *rootObject = engine.rootObjects().first();
        QQuickWindow *window = qobject_cast<QQuickWindow *>(rootObject);
        if (window) {
            window->setGeometry(x, y, width, height);
            //window->show(); // Upewnij się, że okno jest widoczne
            qDebug() << "Ustawiono geometrię okna: x=" << x << ", y=" << y << ", width=" << width << ", height=" << height;
        } else {
            qWarning() << "Root object is not a QQuickWindow!";
        }
    } else {
        qWarning() << "No root objects available to set geometry.";
    }
}

#include "main.moc"

