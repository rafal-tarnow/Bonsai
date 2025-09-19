#include "FrontendManagerService.hpp"
#include <QCryptographicHash>
#include <QDBusConnection>
#include <QDBusError>
#include <QDir>
#include <QUuid>
#include <qdbusmetatype.h>

#include <KSharedConfig>
#include <KConfigGroup>

#include "../maia_version.h"

FrontendManagerService::FrontendManagerService(QObject *parent) :
    QObject(parent)
{
    qDBusRegisterMetaType<QVariantList>();

    QDBusConnection bus = QDBusConnection::sessionBus();

    if(!bus.registerService("org.maia.FrontendManager")){
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
    if (m_activeFrontendId == frontendId)
        return;

    m_activeFrontendId = frontendId;

    saveActiveFronted(m_activeFrontendId);
    emit activeFrontendChanged(m_activeFrontendId);
}

FrontendInfo FrontendManagerService::getCurrentFrontent()
{
    return m_frontends[m_activeFrontendId];
}

QVariantList FrontendManagerService::getFrontendList()
{
    //qDebug() << "Server Call: " << __PRETTY_FUNCTION__;
    QVariantList frontends;
    for(const FrontendInfo &frontend : m_frontends) {
        QVariantMap map;
        map["id"] = frontend.id;
        map["name"] = frontend.name;
        map["description"] = frontend.description;
        map["path"] = frontend.qmlFilePath;
        map["active"] = frontend.id == m_activeFrontendId;
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
    if (frontendId.isEmpty()) {
        return;
    }

    if (m_activeFrontendId == frontendId) {
        return;
    }


    if (!m_frontends.contains(frontendId)) {
        qDebug() << "[ERROR] Frontend with ID" << frontendId << "not found.";
        return;
    }

    emit activeFrontendChangeRequest(m_frontends[frontendId]);
    emit activeFrontendChangeRequest(frontendId);
}

void FrontendManagerService::loadFrontends()
{
    m_frontends.clear();

    FrontendInfo gnomeFrontend;
    gnomeFrontend.name = "Ubuntu 24.04";
    gnomeFrontend.description = "Ubuntu 24.04 like frontend";

#ifdef QT_DEBUG
    // Debug build: use path relative to the build directory
    nomeFrontend.qmlFilePath = "/opt/Maia/Maia_1.0.0/frontends/Gnome/Main.qml";

#else
    // Release build: use absolute path
    gnomeFrontend.qmlFilePath = "/opt/Maia/Maia_1.0.0/frontends/Gnome/Main.qml";

    //gnomeFrontend.qmlFilePath
    //    = "/media/rafal/Bonsai_pendrive/Maia_deploy/Maia_1.0.0/frontends/Gnome/Main.qml";
#endif

    gnomeFrontend.id = QString(
        QCryptographicHash::hash(gnomeFrontend.name.toUtf8(), QCryptographicHash::Sha1).toHex());
    qDebug() << "Gnome Frontend id = " << gnomeFrontend.id;
    m_frontends.insert(gnomeFrontend.id, gnomeFrontend);


    FrontendInfo lunaFrontend;
    lunaFrontend.name = "XP Luna";
    lunaFrontend.description = "XP Luna like frontend";
    lunaFrontend.qmlFilePath = "";
    lunaFrontend.qmlUri = "XPFrontend";
    lunaFrontend.qmlTypeName = "Main";
    lunaFrontend.id = QString(QCryptographicHash::hash(lunaFrontend.name.toUtf8(), QCryptographicHash::Sha1).toHex());
    qDebug() << "Luna XP id = " << lunaFrontend.id;
    m_frontends.insert(lunaFrontend.id, lunaFrontend);


    QString savedFrontendId = readActiveFronted();

    // Ustawiamy aktywny frontend: zapisany, jeśli istnieje i jest ważny, w przeciwnym razie domyślny
    if (!savedFrontendId.isEmpty() && m_frontends.contains(savedFrontendId)) {
        m_activeFrontendId = savedFrontendId;
    } else {
        m_activeFrontendId = m_frontends.isEmpty() ? "" : gnomeFrontend.id; // Domyślny frontend
    }

    //Emitujemy sygnał, jeśli aktywny frontend jest ustawiony
    if (!m_activeFrontendId.isEmpty()) {
        emit activeFrontendChanged(m_activeFrontendId);
    }
}

void FrontendManagerService::addFrontend(const FrontendInfo &frontend)
{
    m_frontends.insert(frontend.id, frontend);
    emit frontendAdded(frontend.id, frontend.name, frontend.description, frontend.qmlFilePath);
}

void FrontendManagerService::removeFrontend(const QString &frontendId)
{
    if (m_frontends.remove(frontendId)) {
        emit frontendRemoved(frontendId);
        if (m_activeFrontendId == frontendId && !m_frontends.isEmpty()) {
            // Use iterator to get an arbitrary key
            setActiveFrontend(m_frontends.firstKey());
        }
    }
}

QString FrontendManagerService::readActiveFronted()
{
#warning "this code is not asynchronous"

    KSharedConfig::Ptr config = KSharedConfig::openConfig(QString("./Maia/maiarc_")
                                                          + MAIA_VERSION_STRING);
    KConfigGroup group = config->group("FrontendManagerService");
    return group.readEntry("activeFrontendId", QString());
}

void FrontendManagerService::saveActiveFronted(const QString &frontedId)
{
#warning "This method is not asynchronous"

    // Zapisujemy activeFrontendId do KSharedConfig
    KSharedConfig::Ptr config = KSharedConfig::openConfig(
        QString("./Maia/maiarc_")
        + MAIA_VERSION_STRING); // Nazwa pliku konfiguracyjnego, np. ~/.config/Maia/maiarc_1.0.0
    KConfigGroup group = config->group("FrontendManagerService");
    group.writeEntry("activeFrontendId", frontedId);
    config->sync(); // Zapewniamy zapis do pliku
}
