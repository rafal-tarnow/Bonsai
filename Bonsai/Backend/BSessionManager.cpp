#include "BSessionManager.hpp"
#include <QDBusPendingReply>
#include <QDebug>

BSessionManager::BSessionManager(QObject *parent)
    : QObject(parent)
{
    qDebug() << __PRETTY_FUNCTION__;
    // Połączenie z interfejsem D-Bus serwera
    m_dbusInterface = new QDBusInterface(
        "org.bonsai.SessionService", // Nazwa serwisu
        "/Session",            // Ścieżka obiektu
        "org.bonsai.SessionInterface", // Interfejs
        QDBusConnection::sessionBus(), // Połączenie z sesyjnym D-Bus
        this
        );

    if (!m_dbusInterface->isValid()) {
        qWarning() << "[ERROR] Nie udało się połączyć z interfejsem D-Bus!";
    }else{
        qDebug() << "[OK] Valid org.bonsai.SessionService d-bus";
    }
}

BSessionManager::~BSessionManager()
{
    delete m_dbusInterface;
}

void BSessionManager::logout()
{
    qDebug() << __PRETTY_FUNCTION__;
    if (m_dbusInterface->isValid()) {
        // Asynchroniczne wywołanie metody logout
        QDBusPendingCall call = m_dbusInterface->asyncCall("logout");
        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
        connect(watcher, &QDBusPendingCallWatcher::finished, this, &BSessionManager::onCallFinished);
    } else {
        qWarning() << "D-Bus nie jest dostępny dla logout!";
    }
}

void BSessionManager::reboot()
{
    qDebug() << __PRETTY_FUNCTION__;
    if (m_dbusInterface->isValid()) {
        // Asynchroniczne wywołanie metody reboot
        QDBusPendingCall call = m_dbusInterface->asyncCall("reboot");
        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
        connect(watcher, &QDBusPendingCallWatcher::finished, this, &BSessionManager::onCallFinished);
    } else {
        qWarning() << "D-Bus nie jest dostępny dla reboot!";
    }
}

void BSessionManager::poweroff()
{
    qDebug() << __PRETTY_FUNCTION__;
    if (m_dbusInterface->isValid()) {
        // Asynchroniczne wywołanie metody poweroff
        QDBusPendingCall call = m_dbusInterface->asyncCall("poweroff");
        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
        connect(watcher, &QDBusPendingCallWatcher::finished, this, &BSessionManager::onCallFinished);
    } else {
        qWarning() << "D-Bus nie jest dostępny dla poweroff!";
    }
}

void BSessionManager::onCallFinished(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<> reply = *watcher;
    if (reply.isError()) {
        qWarning() << "Błąd wywołania D-Bus:" << reply.error().message();
    } else {
        qDebug() << "Wywołanie D-Bus zakończone sukcesem";
    }
    watcher->deleteLater();
}
