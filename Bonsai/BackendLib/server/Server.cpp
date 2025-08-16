#include "Server.hpp"

#include <QDebug>
#include <QQuickWindow>

Server::Server(QGuiApplication *app, int swapIntervalOption)
    : QObject(app)
    , m_sessionService(this)
    , m_applicationModel(this)
    , m_favoriteAppsService(this, &m_applicationModel)
    , m_frontendManagerService(this)
    , m_guiManager(this, app, swapIntervalOption)

{
    qDebug() << __PRETTY_FUNCTION__;

    connect(&m_sessionService, &SessionService::logoutRequest, this, &Server::sessionLogout);

    connect(&m_frontendManagerService,
            &FrontendManagerService::activeFrontendChangeRequest,
            this,
            &Server::handleFrontendChangeRequest);

    connect(&m_guiManager,
            &GuiManager::frontendChanged,
            this,
            &Server::handleGuiManagerFrontendChanged);
}

Server::~Server()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void Server::handleFrontendChangeRequest(const QString &themeId)
{
    qDebug() << "Server 4 " << __PRETTY_FUNCTION__
             << " Server logisc received signal with request to change theme, id=" << themeId;

    m_guiManager.setActiveFrontend(themeId);
}

void Server::sessionLogout()
{
    qDebug() << __PRETTY_FUNCTION__;

    m_guiManager.uninit();

    QCoreApplication::exit(0);
}

void Server::handleGuiManagerFrontendChanged(const QString &frontendId)
{
    qDebug() << "Server 10 " << __PRETTY_FUNCTION__;
    m_frontendManagerService.activeFrontendChangeConfirmation(frontendId);
}
