#pragma once

#include <QObject>
#include <QDBusPendingCallWatcher>

class SessionService : public QObject {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bonsai.SessionInterface")
public:
    explicit SessionService(QObject *parent = nullptr);
    ~SessionService();

public slots:
    void logout();
    void reboot();
    void poweroff();

private slots:
    void onCallFinished(QDBusPendingCallWatcher *watcher);

private:
    void executeLogout();
    void executeReboot();
    void executePoweroff();

signals:
    void logoutRequest();
    void rebootRequest();
    void poweroffRequest();
};
