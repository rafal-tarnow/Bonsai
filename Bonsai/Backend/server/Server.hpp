#pragma once

#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>

#include "FrontendManagerAPIService.hpp"
#include "GuiManager.hpp"
#include "SessionService.hpp"
#include "WindowManagerDBus.hpp"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QGuiApplication *app = nullptr, int swapInterval = 0);
    ~Server();

    void setActiveFrontend(const QString &themeId);

private slots:
    void sessionLogout();

private:
    WindowManagerDBus windowManagerDBus;
    SessionService m_sessionService;
    FrontendManagerService m_frontendManagerService;
    GuiManager m_guiManager;
};
