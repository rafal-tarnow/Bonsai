#pragma once

#include <QObject>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingCall>

class WindowManagerDBus : public QObject {
    Q_OBJECT

public:
    explicit WindowManagerDBus(QObject *parent = nullptr);
    ~WindowManagerDBus() override = default;

    Q_INVOKABLE void unloadEffect(const QString &effectName);
    Q_INVOKABLE void reconfigure();
    Q_INVOKABLE void setAuroraeTheme(const QString &themeName);

signals:
    void effectUnloaded(const QString &effectName);
    void errorOccurred(const QString &effectName, const QString &errorMessage);

private:
    QDBusConnection m_bus;
};
