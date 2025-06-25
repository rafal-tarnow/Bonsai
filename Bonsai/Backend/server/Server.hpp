#pragma once

#include <QObject>
#include <QQmlApplicationEngine>

#include "SessionService.hpp"
#include "FrontendManagerService.hpp"
#include "WindowManagerDBus.hpp"

class Server: public QObject{
    Q_OBJECT
public:
    explicit Server(QQmlApplicationEngine * engine = nullptr);
    ~Server();

    void setActiveFrontend(const QString &themeId);

private:
    QString HOME_ENV;
    QQmlApplicationEngine *m_qmlEngine = nullptr;
    WindowManagerDBus windowManagerDBus;
    SessionService m_sessionService;
    FrontendManagerService m_frontendManagerService;
};
