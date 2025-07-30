#include "SessionService.hpp"

#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>
#include <QCoreApplication>
#include <QDBusMessage>
#include <QDBusPendingReply>

SessionService::SessionService(QObject *parent)
    : QObject(parent) {
    qDebug() << __PRETTY_FUNCTION__;

    QString dbusAddress = qgetenv("DBUS_SESSION_BUS_ADDRESS");

    if (!dbusAddress.isEmpty()) {
        //qDebug() << "DBUS_SESSION_BUS_ADDRESS:" << dbusAddress;
    } else {
        qDebug() << "[Warning] DBUS_SESSION_BUS_ADDRESS is not set.";
    }

    // Rejestrujemy obiekt i jego metody na D-Busie
    QDBusConnection bus = QDBusConnection::sessionBus();

    if (bus.isConnected()) {
        //qDebug() << "Session bus is connected. Name:" << bus.baseService();
        //qDebug() << "Session bus connection name:" << bus.name();
    } else {
        qDebug() << "[Warning] Session bus is not connected.";
    }

    if(!bus.registerService("org.bonsai.SessionService")){
        qWarning() << "[ERROR] Failed to register D-Bus service";
        return;
    }

    if(!bus.registerObject("/Session", this, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)) {
        qWarning() << "[ERROR] Failed to register D-Bus object.";
    }
}

SessionService::~SessionService() {}

void SessionService::logout() {
    qDebug() << "[D-Bus] Logout received";
    qDebug() << "QQQQQQQQQQQQQQQQQQQQQQQQQ";
    emit logoutRequest();
}

void SessionService::reboot() {
    qDebug() << "[D-Bus] Reboot received";
    qDebug() << "QQQQQQQQQQQQQQQQQQQQQQQQQ";
    emit rebootRequest();
    executeReboot();
}

void SessionService::poweroff() {
    qDebug() << "[D-Bus] Poweroff received";
    qDebug() << "QQQQQQQQQQQQQQQQQQQQQQQQQ";
    emit poweroffRequest();
    executePoweroff();
}

void SessionService::onCallFinished(QDBusPendingCallWatcher *watcher)
{
    QString operation = watcher->property("operation").toString();
    QDBusPendingReply<> reply = *watcher;
    if (reply.isError()) {
        qWarning() << "Failed to" << operation << ":" << reply.error().message();
    } else {
        qDebug() << operation << "command sent successfully.";
    }
    watcher->deleteLater();
}

void SessionService::executeReboot()
{
    qDebug() << __PRETTY_FUNCTION__;
    QDBusMessage message = QDBusMessage::createMethodCall(
        "org.freedesktop.login1",
        "/org/freedesktop/login1",
        "org.freedesktop.login1.Manager",
        "Reboot"
        );
    message << true;

    QDBusPendingCall pendingCall = QDBusConnection::systemBus().asyncCall(message);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pendingCall, this);
    watcher->setProperty("operation", "reboot");
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &SessionService::onCallFinished);
}

void SessionService::executePoweroff()
{
    qDebug() << __PRETTY_FUNCTION__;
    QDBusMessage message = QDBusMessage::createMethodCall(
        "org.freedesktop.login1",
        "/org/freedesktop/login1",
        "org.freedesktop.login1.Manager",
        "PowerOff"
        );
    message << true;

    QDBusPendingCall pendingCall = QDBusConnection::systemBus().asyncCall(message);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pendingCall, this);
    watcher->setProperty("operation", "power off");
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &SessionService::onCallFinished);
}
