#include "WindowManagerDBus.hpp"
#include <QDBusPendingCallWatcher>
#include <QDebug>
#include <QDBusPendingReply>

#include <KConfig>
#include <KConfigGroup>

WindowManagerDBus::WindowManagerDBus(QObject *parent)
    : QObject(parent),
    m_bus(QDBusConnection::sessionBus()) // Inicjalizacja połączenia z sesyjnym D-Bus
{
}

void WindowManagerDBus::unloadEffect(const QString &effectName)
{
    //qDebug() << __PRETTY_FUNCTION__ << effectName;
    QDBusMessage message = QDBusMessage::createMethodCall(
        "org.kde.KWin",                // Serwis
        "/Effects",                    // Ścieżka
        "org.kde.kwin.Effects",        // Interfejs
        "unloadEffect"                 // Metoda
        );

    message << effectName;

    QDBusPendingCall pendingCall = m_bus.asyncCall(message);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pendingCall, this);

    connect(watcher, &QDBusPendingCallWatcher::finished, this,
            [this, effectName, watcher]() {
                QDBusPendingReply<> reply = *watcher;
                if (reply.isError()) {
                    emit errorOccurred(effectName, reply.error().message());
                    //qDebug() << "Błąd D-Bus dla efektu" << effectName << ":" << reply.error().message();
                } else {
                    emit effectUnloaded(effectName);
                    //qDebug() << "Efekt" << effectName << "został pomyślnie rozładowany";
                }
                watcher->deleteLater();
            });
}

void WindowManagerDBus::reconfigure()
{
    QDBusMessage message = QDBusMessage::createMethodCall(
        "org.kde.KWin",           // Serwis
        "/KWin",                  // Ścieżka
        "org.kde.KWin",           // Interfejs
        "reconfigure"             // Metoda
        );

    QDBusPendingCall pendingCall = m_bus.asyncCall(message);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pendingCall, this);

    connect(watcher, &QDBusPendingCallWatcher::finished, this,
            [this, watcher]() {
                QDBusPendingReply<> reply = *watcher;
                if (reply.isError()) {
                    qDebug() << "Błąd D-Bus podczas rekonfiguracji:" << reply.error().message();
                } else {
                    qDebug() << "Rekonfiguracja KWin zakończona pomyślnie";
                }
                watcher->deleteLater();
            });
}

void WindowManagerDBus::setAuroraeTheme(const QString &themeName)
{
    qDebug() << __PRETTY_FUNCTION__ << themeName;

#warning "This operation is not asynchnous"

    KConfig kwinConfig("kwinrc");

    KConfigGroup decorationGroup(&kwinConfig, "org.kde.kdecoration2");

    decorationGroup.writeEntry("library", "org.kde.kwin.aurorae");
    decorationGroup.writeEntry("theme", "__aurorae__svg__" + themeName);

    if (decorationGroup.sync()) {
        qDebug() << "Motyw Aurorae" << themeName << "został pomyślnie ustawiony.";
    } else {
        qDebug() << "Błąd podczas zapisywania ustawień motywu Aurorae.";
    }
}
