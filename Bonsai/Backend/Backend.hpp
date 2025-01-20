#pragma once

#include <optional>

#include <QObject>
#include <qqml.h>
#include <QCoreApplication>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QTimer>
#include <QFile>
#include <QtGui/qwindowdefs.h>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

#include "ThemesModel.hpp"

class Backend : public QObject {
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend();

    void setQmlEngine(QQmlApplicationEngine * engine);

    Q_PROPERTY(ThemesModel* themesModel READ getThemesModel CONSTANT)
    ThemesModel* getThemesModel();
    Q_INVOKABLE  void setActiveFrontend(const QString& frontendId);

    //cpu load
    Q_PROPERTY(float cpuLoad READ cpuLoad NOTIFY cpuLoadChanged)
    //turn on cpu load measure
    Q_PROPERTY(bool measureCpuLoad READ measureCpuLoad WRITE setMeasureCpuLoad NOTIFY measureCpuLoadChanged)

    //platform info
    Q_PROPERTY(QString platformName READ platformName NOTIFY platformNameChanged)

    //qt version
    Q_PROPERTY(QString qtVersion READ qtVersion CONSTANT)

    //session
    Q_INVOKABLE void logout();
    Q_INVOKABLE void reboot();
    Q_INVOKABLE void poweroff();

    //
    Q_INVOKABLE void activateWindow (WId win);
    Q_INVOKABLE void minimalizeAllWindows();

    //process
    Q_INVOKABLE void startApplication(const QString & id);
    Q_INVOKABLE void startProcess(const QString & proc);
    Q_INVOKABLE void runCommand(const QString & cmd);

    //panelLocation
    // Panel location
    Q_INVOKABLE void reservePanelLeftArea(QQuickWindow * window, int x, int y, int width, int height);
    void reservePanelLeftArea(WId windowId, int x, int y, int width, int height);
    void reservePanelLeftArea(WId windowId, int start_y, int stop_y, int width);

    Q_INVOKABLE void reservePanelTopArea(QQuickWindow * window, int x, int y, int width, int height);
    void reservePanelTopArea(WId windowId, int x, int y, int width, int height);
    void reservePanelTopArea(WId windowId, int start_x, int stop_x, int height);

    Q_INVOKABLE void reservePanelBottomArea(QQuickWindow * window, int x, int y, int width, int height);
    void reservePanelBottomArea(WId windowId, int x, int y, int width, int height);
    void reservePanelBottomArea(WId windowId, int start_x, int stop_x, int height);

    Q_INVOKABLE void setX11WindowTypeAsDesktop(QQuickWindow *window);
    Q_INVOKABLE void setX11WindowTypeAsDock(QQuickWindow *window);

    //platform info
    QString platformName() const;

    //qt Version
    QString qtVersion() const;

    //cpu load
    float cpuLoad() const;

    //measure cpu load flag
    bool measureCpuLoad() const;
    void setMeasureCpuLoad(bool enable);

private slots:
    void updateCpuLoad();

signals:
    void platformNameChanged();
    void cpuLoadChanged();
    void measureCpuLoadChanged();

private:
    QQmlApplicationEngine *qmlEngine = nullptr;
    QString m_platformName;
    ThemesModel themesModel;

    //cpu load variables
    float m_cpuLoad = 0.0f;
    QTimer m_timer;
    long long prevTotal = 0;
    long long prevIdle = 0;

    // File for reading CPU load
    QFile m_cpuFile;

    // Flag to control CPU load measurement
    bool m_measureCpuLoad = false;
};
