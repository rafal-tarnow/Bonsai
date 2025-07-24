#include "X11WindowManagerService.hpp"

#include <QDBusConnection>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>

#include <KX11Extras>

WindowManagerX11Service::WindowManagerX11Service(QObject *parent)
    : QObject(parent)
    , m_kwinBus(QDBusConnection::sessionBus())
{
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

    if (!bus.registerService("org.bonsai.WindowManagerX11Service")) {
        qWarning() << "[ERROR] Failed to register D-Bus service";
        return;
    }

    if (!bus.registerObject("/WindowManagerX11",
                            this,
                            QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)) {
        qWarning() << "[ERROR] Failed to register D-Bus object.";
    }

    //----------------------------------------

    QDBusConnection::sessionBus().connect("org.kde.KWin",                // Nazwa serwisu
                                          "/KWin",                       // Ścieżka obiektu
                                          "org.kde.KWin",                // Nazwa interfejsu
                                          "reloadConfig",                // Nazwa sygnału
                                          this,                          // Obiekt odbiorcy
                                          SLOT(handleKwinConfigReloaded) // Slot, który ma być wywołany
    );
}

WindowManagerX11Service::~WindowManagerX11Service() {}

void WindowManagerX11Service::unloadEffect(const QString &effectName)
{
    qDebug() << __PRETTY_FUNCTION__ << " effectName = " << effectName;

    //qDebug() << __PRETTY_FUNCTION__ << effectName;
    QDBusMessage message = QDBusMessage::createMethodCall("org.kde.KWin",         // Serwis
                                                          "/Effects",             // Ścieżka
                                                          "org.kde.kwin.Effects", // Interfejs
                                                          "unloadEffect"          // Metoda
    );

    message << effectName;

    QDBusPendingCall pendingCall = m_kwinBus.asyncCall(message);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pendingCall, this);

    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this, effectName, watcher]() {
        QDBusPendingReply<> reply = *watcher;
        if (reply.isError()) {
            //emit errorOccurred(effectName, reply.error().message());
            //qDebug() << "Błąd D-Bus dla efektu" << effectName << ":" << reply.error().message();
        } else {
            //emit effectUnloaded(effectName);
            //qDebug() << "Efekt" << effectName << "został pomyślnie rozładowany";
        }
        watcher->deleteLater();
    });
}

void WindowManagerX11Service::reconfigure()
{
    qDebug() << __PRETTY_FUNCTION__;

    QDBusMessage message = QDBusMessage::createMethodCall("org.kde.KWin", // Serwis
                                                          "/KWin",        // Ścieżka
                                                          "org.kde.KWin", // Interfejs
                                                          "reconfigure"   // Metoda
    );

    // Utwórz i uruchom timer
    QElapsedTimer timer;
    timer.start();

    QDBusPendingCall pendingCall = m_kwinBus.asyncCall(message);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pendingCall, this);

    // connect(watcher, &QDBusPendingCallWatcher::finished, this, [this, watcher]() {
    //     qDebug() << ">>> Sygnał 'finished' otrzymany po:" << elapsed_ms << "ms (" << elapsed_us
    //              << "µs)";

    //     QDBusPendingReply<> reply = *watcher;
    //     if (reply.isError()) {
    //         qDebug() << "Błąd D-Bus podczas rekonfiguracji:" << reply.error().message();
    //     } else {
    //         qDebug() << "Rekonfiguracja KWin zakończona pomyślnie";
    //     }
    //     watcher->deleteLater();
    // });

    connect(watcher, &QDBusPendingCallWatcher::finished, this, [=]() mutable {
        // Zmierz i wyświetl czas, jaki upłynął od momentu timer.start()
        qint64 elapsed_ms = timer.elapsed();
        double elapsed_us = timer.nsecsElapsed() / 1000.0; // w mikrosekundach dla większej precyzji

        qDebug() << ">>> Sygnał 'finished' otrzymany po:" << elapsed_ms << "ms (" << elapsed_us
                 << "µs)";

        QDBusPendingReply<> reply = *watcher;
        if (reply.isError()) {
            qDebug() << "Błąd D-Bus podczas rekonfiguracji:" << reply.error().message();
        } else {
            // Ten komunikat jest mylący, ale zostawmy go na potrzeby testu
            qDebug() << "Komunikat z kodu: Rekonfiguracja KWin zakończona pomyślnie.";
        }
        watcher->deleteLater();
        qDebug() << "--------------------------------------------------";
    });
}

void WindowManagerX11Service::hideFromTaskbar(WId winId, bool hide)
{
    qDebug() << __PRETTY_FUNCTION__ << " windId=" << winId << " hide=" << hide;
    if (hide)
        KX11Extras::setState(winId, NET::SkipTaskbar);
    else
        KX11Extras::clearState(winId, NET::SkipTaskbar);
}

void WindowManagerX11Service::hideFromPager(WId winId, bool hide)
{
    qDebug() << __PRETTY_FUNCTION__ << " windId=" << winId << " hide=" << hide;
    if (hide)
        KX11Extras::setState(winId, NET::SkipPager);
    else
        KX11Extras::clearState(winId, NET::SkipPager);
}

void WindowManagerX11Service::hideFromSwitcher(WId winId, bool hide)
{
    qDebug() << __PRETTY_FUNCTION__ << " windId=" << winId << " hide=" << hide;
    if (hide)
        KX11Extras::setState(winId, NET::SkipSwitcher);
    else
        KX11Extras::clearState(winId, NET::SkipSwitcher);
}

void WindowManagerX11Service::handleKwinConfigReloaded()
{
    qDebug() << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
                "XXXXXXXXXXXXXXXXXXXXXXX "
             << __PRETTY_FUNCTION__;
}
