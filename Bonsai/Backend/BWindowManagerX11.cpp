#include "BWindowManagerX11.hpp"

#include <QDBusPendingReply>

BWindowManagerX11::BWindowManagerX11(QObject *parent)
{
    qDebug() << __PRETTY_FUNCTION__;

    m_dbusInterface = new QDBusInterface("org.bonsai.WindowManagerX11Service",
                                         "/WindowManagerX11",
                                         "org.bonsai.WindowManagerX11Interface",
                                         QDBusConnection::sessionBus(),
                                         this);

    if (!m_dbusInterface->isValid()) {
        qWarning() << "[ERROR] Nie udało się połączyć z interfejsem D-Bus!";
    } else {
        qDebug() << "[OK] Valid org.bonsai.SessionService d-bus";
    }
}

BWindowManagerX11::~BWindowManagerX11()
{
    delete m_dbusInterface;
}

void BWindowManagerX11::reconfigure()
{
    qDebug() << __PRETTY_FUNCTION__;

    if (!m_dbusInterface->isValid()) {
        qWarning() << "D-Bus nie jest dostępny dla logout!";
        return;
    }

    QDBusPendingCall call = m_dbusInterface->asyncCall("reconfigure");
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &BWindowManagerX11::onCallFinished);
}

void BWindowManagerX11::unloadEffect(const QString &effectName)
{
    qDebug() << __PRETTY_FUNCTION__;

    if (!m_dbusInterface->isValid()) {
        qWarning() << "D-Bus nie jest dostępny dla logout!";
        return;
    }

    QDBusPendingCall call = m_dbusInterface->asyncCall("unloadEffect", effectName);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &BWindowManagerX11::onCallFinished);
}

void BWindowManagerX11::hideFromTaskbar(const QWindow *window, const bool hide)
{
    qDebug() << __PRETTY_FUNCTION__;

    if (!window)
        return;

    if (!m_dbusInterface->isValid()) {
        qWarning() << "D-Bus nie jest dostępny dla logout!";
        return;
    }

    QDBusPendingCall call = m_dbusInterface->asyncCall("hideFromTaskbar", window->winId(), hide);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &BWindowManagerX11::onCallFinished);
}

void BWindowManagerX11::hideFromPager(const QWindow *window, const bool hide)
{
    qDebug() << __PRETTY_FUNCTION__;

    if (!window)
        return;

    if (!m_dbusInterface->isValid()) {
        qWarning() << "D-Bus nie jest dostępny dla logout!";
        return;
    }

    QDBusPendingCall call = m_dbusInterface->asyncCall("hideFromPager", window->winId(), hide);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &BWindowManagerX11::onCallFinished);
}

void BWindowManagerX11::hideFromSwitcher(const QWindow *window, const bool hide)
{
    qDebug() << __PRETTY_FUNCTION__;

    if (!window)
        return;

    if (!m_dbusInterface->isValid()) {
        qWarning() << "D-Bus nie jest dostępny dla logout!";
        return;
    }

    QDBusPendingCall call = m_dbusInterface->asyncCall("hideFromSwitcher", window->winId(), hide);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &BWindowManagerX11::onCallFinished);
}

void BWindowManagerX11::onCallFinished(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<> reply = *watcher;
    if (reply.isError()) {
        qWarning() << "Błąd wywołania D-Bus:" << reply.error().message();
    } else {
        qDebug() << "Wywołanie D-Bus zakończone sukcesem";
    }
    watcher->deleteLater();
}
