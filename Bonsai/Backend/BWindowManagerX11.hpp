#pragma once

#include <QDBusInterface>
#include <QObject>
#include <qqml.h>

#include <QWindow>

class BWindowManagerX11 : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    BWindowManagerX11(QObject *parent = nullptr);
    ~BWindowManagerX11();

    Q_INVOKABLE void reconfigure();
    Q_INVOKABLE void unloadEffect(const QString &effectName);
    Q_INVOKABLE void hideFromTaskbar(const QWindow *window, const bool hide);
    Q_INVOKABLE void hideFromPager(const QWindow *window, const bool hide);
    Q_INVOKABLE void hideFromSwitcher(const QWindow *window, const bool hide);

private slots:
    void onCallFinished(QDBusPendingCallWatcher *watcher);

private:
    QDBusInterface *m_dbusInterface;
};
