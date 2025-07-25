#include <csignal>
#include <memory>

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDir>
#include <QGuiApplication>
#include <QObject>
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardPaths>
#include <QWindow>
#include <QtMessageHandler>
#include <QtQuick3D/qquick3d.h>
#include <QtWebEngineQuick>

#include <AppsListModel.hpp>
#include <AudioBackend.hpp>
#include <BSessionManager.hpp>
#include <Backend.hpp>
#include <BackendAppsIconsProvider.hpp>
#include <FavoriteAppsListModel.hpp>
#include <TaskbarIconProvider.hpp>
#include <helper/Process.hpp>
#include <private/ProxyWindowServer.hpp>
#include <server/Server.hpp>

#include "logger.hpp"

#include <QtQml/QQmlExtensionPlugin>
Q_IMPORT_QML_PLUGIN(XPFrontendPlugin)

void getCmdLineOptions(const QCoreApplication &app,
                       QString &modeOption,
                       QString &sourceOption,
                       int &x,
                       int &y,
                       int &width,
                       int &height,
                       int &swapInterval,
                       QString &proxyWindowAddress);

static void signalHandler(int signal)
{
    qDebug() << "Otrzymano sygnał:" << signal << ". Zamykam aplikację...";
    QCoreApplication::quit();
}

void setWindowGeometry(QQmlApplicationEngine &engine, int x, int y, int width, int height);

int main(int argc, char *argv[])
{
    // Rejestracja obsługi sygnałów systemowych
    std::signal(SIGTERM, signalHandler); // Sygnał zakończenia (np. z Qt Creatora lub kill)
    std::signal(SIGINT, signalHandler);  // Ctrl+C w terminalu

    //READ ENVIROMENT VARIABLES
    QString homePath = qgetenv("HOME");
    QString gnomeModulePath = homePath + "/Bonsai/themes/gnome";

    //INIT APPLICATION
    QGuiApplication::setApplicationName("bonsai");
    QGuiApplication::setOrganizationName("Bonsai");
    QtWebEngineQuick::initialize();

    QGuiApplication app(argc, argv);

    Logger logger;
    logger.init();
    auto originalHandler = qInstallMessageHandler(Logger::messageHandler);
    logger.setOriginalHandler(originalHandler);

    //INIT APPLICATION OPTIONS
    QString modeOption, sourceOption, proxyWinAddress;
    int xOption, yOption, widthOption, heightOption, swapInterwalOption;
    getCmdLineOptions(app,
                      modeOption,
                      sourceOption,
                      xOption,
                      yOption,
                      widthOption,
                      heightOption,
                      swapInterwalOption,
                      proxyWinAddress);

    qDebug() << "[INFO] " << "Proxy Window Address: " << proxyWinAddress;

    if (modeOption == "server") {
        //SERVER

        Server server(&app, swapInterwalOption);
        return app.exec();

    } else {
        //CLIENT

        //INIT SURFACE
        QSurfaceFormat surfaceFormat = QQuick3D::idealSurfaceFormat();
        //QSurfaceFormat surfaceFormat = QSurfaceFormat::defaultFormat();
        surfaceFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
        surfaceFormat.setSwapInterval(swapInterwalOption);
        QSurfaceFormat::setDefaultFormat(surfaceFormat);

        QQmlApplicationEngine engine;
        engine.addImportPath(gnomeModulePath);

        Backend backend(homePath);
        backend.setQmlEngine(&engine);

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

        ProxyWindowLocalServer server;
        if (!server.startServer(proxyWinAddress)) {
            return -1;
        }

        engine.rootContext()->setContextProperty("HOME", homePath);
        //Apps List models
        engine.rootContext()->setContextProperty("appsListModel", &filterProxyModel);
        engine.rootContext()->setContextProperty("favoriteAppsModel", &favoriteAppsModel);

        engine.rootContext()->setContextProperty("backend", &backend);

        //Audio backend
        AudioBackend audioBackend;
        engine.rootContext()->setContextProperty("audioBackend", &audioBackend);

        QObject::connect(
            &engine,
            &QQmlApplicationEngine::objectCreationFailed,
            &app,
            []() { QCoreApplication::exit(-1); },
            Qt::QueuedConnection);

        engine.load(sourceOption);

        //Make sure the main QML object is loaded.
        if (engine.rootObjects().isEmpty())
            return -1;

        setWindowGeometry(engine, xOption, yOption, widthOption, heightOption);
        server.installWindow(engine);

        auto retVal = app.exec();
        qDebug() << "ĘĘĘĘĘĘĘĘĘĘĘĘĘĘĘĘĘĘĘĘĘĘĘĘĘĘĘĘĘ CLIENT extit with retVal = " << retVal;
        logger.uninit();
        return retVal;
    }
}

void getCmdLineOptions(const QCoreApplication &app,
                       QString &modeOption,
                       QString &sourceOption,
                       int &x,
                       int &y,
                       int &width,
                       int &height,
                       int &swapInterval,
                       QString &proxyWindowAddress)
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
    QCommandLineOption swapIntervalOpt("swap-interval", "Set swap interval", "value", "1");
    QCommandLineOption proxyWinAddressOpt("proxy-window-addr",
                                          "Set proxy window local socket address",
                                          "value",
                                          "/tmp/bonsai-XYZ");

    // Add options to parser
    parser.addOptions(
        {modeOpt, sourceOpt, xOpt, yOpt, widthOpt, heightOpt, swapIntervalOpt, proxyWinAddressOpt});

    // Process arguments
    parser.process(app);

    // Set default values
    modeOption = parser.value(modeOpt);
    sourceOption = parser.value(sourceOpt);
    proxyWindowAddress = parser.value(proxyWinAddressOpt);
    x = 100;
    y = 100;
    width = 800;
    height = 600;
    swapInterval = 0;

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
                qWarning() << "Invalid" << name << "value:" << parser.value(opt)
                           << "(expected: non-negative integer)";
            }
        }
    };

    parseIntOption(xOpt, x, "--x");
    parseIntOption(yOpt, y, "--y");
    parseIntOption(widthOpt, width, "--width");
    parseIntOption(heightOpt, height, "--height");
    parseIntOption(swapIntervalOpt, swapInterval, "--swap-interval"); // Parsowanie swap-interval
}

// void startKwinAndWaitForReady(QProcess &process, int timeoutMs)
// {
//     //START KWIN
//     QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
//     filterProcessEnvironment(env, "/opt/Bonsai/DistributionKit_2");
//     process.setProcessEnvironment(env);
//     process.start("kwin");
//     process.waitForStarted(5000);
// }

void setWindowGeometry(QQmlApplicationEngine &engine, int x, int y, int width, int height)
{
    if (engine.rootObjects().isEmpty()) {
        qWarning() << "No root objects available to set geometry.";
    }

    QObject *rootObject = engine.rootObjects().first();
    QQuickWindow *window = qobject_cast<QQuickWindow *>(rootObject);

    if (!window) {
        qWarning() << "Root object is not a QQuickWindow!";
    }

    window->setGeometry(x, y, width, height);
    //window->show(); // Upewnij się, że okno jest widoczne
    qDebug() << "Ustawiono geometrię okna: x=" << x << ", y=" << y << ", width=" << width
             << ", height=" << height;
}

#include "main.moc"
