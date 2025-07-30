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
