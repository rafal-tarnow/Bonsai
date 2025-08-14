#include "FrontendManagerService.hpp"
#include <QCryptographicHash>
#include <QDBusConnection>
#include <QDBusError>
#include <QDir>
#include <QUuid>
#include <qdbusmetatype.h>

FrontendManagerService::FrontendManagerService(QObject *parent) :
    QObject(parent)
{
    qDBusRegisterMetaType<QVariantList>();

    QDBusConnection bus = QDBusConnection::sessionBus();

    if(!bus.registerService("org.bonsai.FrontendManager")){
        qFatal("Failed to register D-Bus service: %s", qPrintable(bus.lastError().message()));
    }

    //register FronendManager object
    if (!bus.registerObject("/FrontendManager",
                            this,
                            QDBusConnection::ExportAllSlots
                                | QDBusConnection::ExportScriptableSignals
                                | QDBusConnection::ExportAllProperties)) {
        qFatal("Failed to register D-Bus object: %s", qPrintable(bus.lastError().message()));
    }

    loadFrontends();
}

FrontendManagerService::~FrontendManagerService()
{

}

void FrontendManagerService::activeFrontendChangeConfirmation(const QString &frontendId)
{
    //qDebug() << "Server 11 " << __PRETTY_FUNCTION__;
    if (m_activeFrontendId == frontendId)
        return;

    m_activeFrontendId = frontendId;

    for (Frontend &frontend : m_frontends) { // Note: Removed const to allow modification
        frontend.active = (frontend.id == m_activeFrontendId);
    }
    //qDebug()
    //    << "Server 12 "
    //    << " ******************************  D-Bus signal: emit activeFrontendChanged(frontendId);";
    emit activeFrontendChanged(frontendId);
}

QVariantList FrontendManagerService::getFrontendList()
{
    //qDebug() << "Server Call: " << __PRETTY_FUNCTION__;
    QVariantList frontends;
    for(const Frontend &frontend : m_frontends) {
        QVariantMap map;
        map["id"] = frontend.id;
        map["name"] = frontend.name;
        map["description"] = frontend.description;
        map["path"] = frontend.path;
        map["active"] = frontend.active;
        frontends.append(map);
    }
    return frontends;
}

QString FrontendManagerService::activeFrontend() const
{
    //qDebug() << "Server Call: " << __PRETTY_FUNCTION__;
    return m_activeFrontendId;
}

//d-bus interface
void FrontendManagerService::setActiveFrontend(const QString &frontendId)
{
    // qDebug() << "Server 2 " << __PRETTY_FUNCTION__
    //          << "Server side, dbus received , frontendId=" << frontendId;
    if (frontendId.isEmpty()) {
        // qDebug() << "3.1-- " << __PRETTY_FUNCTION__;
        // qDebug() << "Invalid frontend ID: empty";
        return;
    }

    if (m_activeFrontendId == frontendId) {
        //qDebug() << "3.3-- " << __PRETTY_FUNCTION__;
        return;
    }

    //qDebug() << "Server 3 " << __PRETTY_FUNCTION__ << " emit activeFrontendChangeRequest";
    emit activeFrontendChangeRequest(frontendId);
}

void FrontendManagerService::loadFrontends()
{
    // QDir dir("frontends");
    // QStringList frontendFiles = dir.entryList(QStringList() << "*.qml", QDir::Files);

    // m_frontends.clear();
    // for(const QString &file : frontendFiles) {
    //     Frontend frontend;
    //     frontend.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    //     frontend.name = file.section(".", 0, 0);
    //     frontend.description = "Frontend: " + frontend.name;
    //     frontend.path = dir.absoluteFilePath(file);
    //     m_frontends.append(frontend);
    // }

    Frontend frontend;
    frontend.name = "Gnome";
    frontend.description = "Ubuntu Gnome like frontend";
    frontend.path = "/home/rafal/Bonsai/thmes/gnome/";
    frontend.id = QString(QCryptographicHash::hash(frontend.name.toUtf8(), QCryptographicHash::Sha1).toHex());
    qDebug() << "Gnome Frontend id = " << frontend.id;
    frontend.active = false;

    m_frontends.append(frontend);

    frontend.name = "XP Luna";
    frontend.description = "XP Luna like frontend";
    frontend.path = "/home/rafal/Bonsai/thmes/xp_luna/";
    frontend.id = QString(QCryptographicHash::hash(frontend.name.toUtf8(), QCryptographicHash::Sha1).toHex());
    qDebug() << "Luna XP id = " << frontend.id;
    frontend.active = true;

    m_frontends.append(frontend);

    m_activeFrontendId = frontend.id;
}

void FrontendManagerService::addFrontend(const Frontend &frontend)
{
    m_frontends.append(frontend);
    emit frontendAdded(frontend.id, frontend.name, frontend.description, frontend.path);
}

void FrontendManagerService::removeFrontend(const QString &frontendId)
{
    for(int i = 0; i < m_frontends.size(); ++i){
        if(m_frontends[i].id == frontendId){
            m_frontends.removeAt(i);
            emit frontendRemoved(frontendId);
            if(m_activeFrontendId == frontendId && !m_frontends.isEmpty()){
                setActiveFrontend(m_frontends.first().id);
            }
            return;
        }
    }
}
