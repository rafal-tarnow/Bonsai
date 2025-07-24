#include "Server.hpp"

#include <QDebug>
#include <QQuickWindow>

Server::Server(QGuiApplication *app, int swapIntervalOption)
    : QObject(app)
    , m_sessionService(this)
    , m_frontendManagerService(this)
    , m_guiManager(this, app, swapIntervalOption)

{
    qDebug() << __PRETTY_FUNCTION__;

    connect(&m_sessionService, &SessionService::logoutRequest, this, &Server::sessionLogout);

    // Connect to frontendAdded signal
    // QObject::connect(&m_frontendManagerService, &FrontendManagerService::frontendAdded,
    //                  [](const QString& id, const QString& name,
    //                     const QString& description, const QString& path) {
    //                      qDebug() << "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
    //                      qDebug() << "Frontend added - ID:" << id
    //                               << "Name:" << name
    //                               << "Description:" << description
    //                               << "Path:" << path;
    //                  });

    // Connect to activeFrontendChanged signal
    QObject::connect(&m_frontendManagerService,
                     &FrontendManagerService::activeFrontendChanged,
                     [](const QString &frontendId) {
                         qDebug() << "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
                         qDebug() << "Active frontend changed to:" << frontendId;
                     });

    QObject::connect(&m_frontendManagerService,
                     &FrontendManagerService::activeFrontendChangeRequest,
                     this,
                     &Server::setActiveFrontend);
}

Server::~Server() {}

void Server::setActiveFrontend(const QString &themeId)
{
    qDebug() << __PRETTY_FUNCTION__
             << " Server logisc received signal with request to change theme, id=" << themeId;

    m_guiManager.setActiveFrontend(themeId);

    m_frontendManagerService.activeFrontendChangeConfirmation(themeId);
}

void Server::sessionLogout()
{
    qDebug() << __PRETTY_FUNCTION__;

    m_guiManager.uninit();

    QCoreApplication::exit(0);
}
