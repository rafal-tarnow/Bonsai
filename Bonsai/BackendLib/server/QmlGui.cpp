#include "QmlGui.hpp"

#include <QQmlContext>
#include <QSurface>

QmlGui::QmlGui(QObject *parent, QGuiApplication *app, int swapIntervalOption)
    : QObject(parent)
    , m_app(app)
    , m_swapIntervalOption(swapIntervalOption)
    , backend(qgetenv("HOME"))
{}

void QmlGui::initQmlEngine()
{
    //READ ENVIROMENT VARIABLES
    QString homePath = qgetenv("HOME");
    //QString gnomeModulePath = homePath + "/Bonsai/themes/gnome";
    QString gnomeModulePath = homePath + "/opt/Bonsai/Bonsai_1.0.0/frontends/Gnome";


    //INIT SURFACE
    //QSurfaceFormat surfaceFormat = QQuick3D::idealSurfaceFormat();
    QSurfaceFormat surfaceFormat = QSurfaceFormat::defaultFormat();
    surfaceFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    surfaceFormat.setSwapInterval(m_swapIntervalOption);
    QSurfaceFormat::setDefaultFormat(surfaceFormat);

    engine.addImportPath(gnomeModulePath);

    backend.setQmlEngine(&engine);

    //INIT QML ENGINE ICON PROVIDERS
    engine.addImageProvider(QLatin1String("backend_app_icon"), &appsIconProvider);
    engine.addImageProvider(QLatin1String("backendTaskbarIcons"), &taskbarIconProvider);

    //INIT QML ENGINE CONTEXT PROPERTIES

    filterProxyModel.setSourceModel(&appsListModel);

    engine.rootContext()->setContextProperty("HOME", homePath);
    //Apps List models
    engine.rootContext()->setContextProperty("appsListModel", &filterProxyModel);

    engine.rootContext()->setContextProperty("backend", &backend);

    //Audio backend
    engine.rootContext()->setContextProperty("audioBackend", &audioBackend);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        m_app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
}

void QmlGui::deleteQmlEngineRootObjects()
{
    qDebug() << "Server 6" << __PRETTY_FUNCTION__;
    //close all root QQuickWindows, if we dont close all windows, when we switch frontend, thhey sometimes stay some windows, for example some side panel windows
    for (QObject *rootObject : engine.rootObjects()) {
        if (QQuickWindow *window = qobject_cast<QQuickWindow *>(rootObject)) {
            //window->close();
            //window->deleteLater();
        }
        rootObject->deleteLater();
    }

    qDebug() << "Server 6" << "engine.clearComponentCache();";
    engine.clearComponentCache();
}

void QmlGui::loadFromModule(QAnyStringView uri, QAnyStringView typeName)
{
    qDebug() << "Server 8 " << __PRETTY_FUNCTION__;
    engine.loadFromModule(uri, typeName);

    //Make sure the main QML object is loaded.
    if (engine.rootObjects().isEmpty()) {
        qDebug() << "QCoreApplication::quit() 1";
        QCoreApplication::quit();
    }
}

void QmlGui::load(const QString &filePath)
{
    qDebug() << "Server 7 " << __PRETTY_FUNCTION__;
    engine.load(filePath);

    //Make sure the main QML object is loaded.
    if (engine.rootObjects().isEmpty()) {
        qDebug() << "QCoreApplication::quit() 2";
        QCoreApplication::quit();
    }
}
