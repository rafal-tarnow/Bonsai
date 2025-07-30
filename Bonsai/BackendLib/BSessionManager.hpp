#pragma once

#include <QObject>
#include <QDBusInterface>
#include <QDBusPendingCallWatcher>
#include <qqml.h>

class BSessionManager : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    BSessionManager(QObject *parent = nullptr);
    ~BSessionManager();

    Q_INVOKABLE void logout();
    Q_INVOKABLE void reboot();
    Q_INVOKABLE void poweroff();

private:
    QDBusInterface *m_dbusInterface;

private slots:
    void onCallFinished(QDBusPendingCallWatcher *watcher);
};
