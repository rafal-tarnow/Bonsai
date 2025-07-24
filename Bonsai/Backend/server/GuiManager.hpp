#pragma once

#include <QObject>
#include <QProcess>

#include "QmlGui.hpp"
#include "X11WindowManagerService.hpp"

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
    void frontendChanged();

private:
    QString HOME_ENV;
    WindowManagerX11Service m_x11WindowManagerService;
    QmlGui m_qmlGui;
    QProcess m_kwin;
};
