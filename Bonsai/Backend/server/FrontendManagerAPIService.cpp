#include "FrontendManagerAPIService.hpp"
#include <QDBusConnection>
#include <QDBusError>
#include <QDir>
#include <QUuid>
#include <QCryptographicHash>
#include <qdbusmetatype.h>

FrontendManagerService::FrontendManagerService(QObject *parent) :
    QObject(parent)
{
    qDBusRegisterMetaType<QVariantList>();

    QDBusConnection bus = QDBusConnection::sessionBus();

    //register FrontendManager service
    if(!bus.registerService("org.bonsai.FrontendManager")){
        qFatal("Failed to register D-Bus service: %s", qPrintable(bus.lastError().message()));
    }

    //register FronendManager object
    if(!bus.registerObject("/FrontendManager", this, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals | QDBusConnection::ExportAllProperties)){
        qFatal("Failed to register D-Bus object: %s", qPrintable(bus.lastError().message()));
    }

    loadFrontends();
}

FrontendManagerService::~FrontendManagerService()
{

}

void FrontendManagerService::activeFrontendChangeConfirmation(const QString &frontendId)
{
    if (m_activeFrontendId == frontendId)
        return;

    m_activeFrontendId = frontendId;

    for (Frontend &frontend : m_frontends) { // Note: Removed const to allow modification
        frontend.active = (frontend.id == frontendId);
    }

    emit activeFrontendChanged(frontendId);
}

QVariantList FrontendManagerService::getFrontendList()
{
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

    return m_activeFrontendId;
}

//d-bus interface
void FrontendManagerService::setActiveFrontend(const QString &frontendId)
{
    qDebug() << __PRETTY_FUNCTION__ << "Server side, dbus received , frontendId=" << frontendId;
    if (frontendId.isEmpty()) {
        qDebug() << "3.1-- " << __PRETTY_FUNCTION__;
        qDebug() << "Invalid frontend ID: empty";
        return;
    }

    qDebug() << "3.2-- " << __PRETTY_FUNCTION__;
    if (m_activeFrontendId == frontendId) {
        qDebug() << "3.3-- " << __PRETTY_FUNCTION__;
        return;
    }

    qDebug() << __PRETTY_FUNCTION__ << " emit activeFrontendChangeRequest";
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
    frontend.active = false;

    m_frontends.append(frontend);

    frontend.name = "XP Luna";
    frontend.description = "XP Luna like frontend";
    frontend.path = "/home/rafal/Bonsai/thmes/xp_luna/";
    frontend.id = QString(QCryptographicHash::hash(frontend.name.toUtf8(), QCryptographicHash::Sha1).toHex());
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
