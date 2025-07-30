#pragma once

#include <QObject>
#include <QProcess>

#include "QmlGui.hpp"
#include "X11WindowManagerService.hpp"

//generated scxml classes
//#include "GuiManagerStateMachine.h"
#include <QScxmlStateMachine>
#include "GuiManagerStateMachine.h"

class GuiManager : public QObject
{
    Q_OBJECT
public:
    explicit GuiManager(QObject *parent = nullptr,
                        QGuiApplication *app = nullptr,
                        int swapIntervalOption = 0);
    ~GuiManager();

    void setActiveFrontend(const QString &themeId);

    void uninit();

signals:
    void frontendChanged(const QString frontendId);

private slots:
    void handleKwinReconfigured();

private:
    QString HOME_ENV;
    WindowManagerX11Service m_x11WindowManagerService;
    QmlGui m_qmlGui;
    QProcess m_kwin;
    QString m_currentThemeId;
    GuiManagerStateMachine changeFrontendStateMachine;
};
