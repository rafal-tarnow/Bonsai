#include "Server.hpp"

#include <QDebug>
#include <QQuickWindow>

Server::Server(QQmlApplicationEngine * engine):
    QObject(engine),
    m_sessionService(this),
    m_frontendManagerService(this),
    m_qmlEngine(engine)
{
    qDebug() << __PRETTY_FUNCTION__;
    HOME_ENV = qgetenv("HOME");

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
    QObject::connect(&m_frontendManagerService, &FrontendManagerService::activeFrontendChanged,
                     [](const QString& frontendId) {
                         qDebug() << "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
                         qDebug() << "Active frontend changed to:" << frontendId;
                     });

    // QObject::connect(&m_frontendManagerService, &FrontendManagerService::activeFrontendChangeRequest,
    //                  [](const QString& frontendId){
    //                      qDebug() << "5-- " << "lambda activeFrontendChangeRequest ";

    //                  });

    QObject::connect(&m_frontendManagerService, &FrontendManagerService::activeFrontendChangeRequest, this, &Server::setActiveFrontend);
}

Server::~Server()
{

}

void Server::setActiveFrontend(const QString &themeId)
{
    qDebug() << "5-- " << __PRETTY_FUNCTION__ << " " << themeId;

    if(m_qmlEngine){

        //close all root QQuickWindows, if we dont close all windows, when we switch frontend, thhey sometimes stay some windows, for example some side panel windows
        for (QObject* rootObject : m_qmlEngine->rootObjects()) {
            if (QQuickWindow* window = qobject_cast<QQuickWindow*>(rootObject)) {
                window->close();
                window->deleteLater();
            }
        }

        m_qmlEngine->clearComponentCache();

        if(themeId == "862b771db54b6d6764f8d72ece0e31e994bf0058"){
            m_qmlEngine->load(HOME_ENV + "/Bonsai/themes/gnome/Main.qml");
        }else if(themeId == "351c97c34271d2a3ac6c180b951c920528de19d8"){ //XP Luna
            m_qmlEngine->loadFromModule("XPFrontend", "Main");
        }

        //reset kwin state
        windowManagerDBus.reconfigure();
        m_frontendManagerService.activeFrontendChangeConfirmation(themeId);
    }
}

