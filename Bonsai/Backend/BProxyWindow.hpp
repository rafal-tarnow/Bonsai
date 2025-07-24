#pragma once

#include <QQuickItem>
#include <QProcess>
#include <QQuickWindow>

#include "./private/ProxyWindowController.hpp"

class BProxyWindow : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(int swapInterval READ swapInterval WRITE setSwapInterval NOTIFY swapIntervalChanged)
    Q_PROPERTY(bool proxyVisible READ proxyVisible WRITE setProxyVisible NOTIFY proxyVisibleChanged)

public:
    explicit BProxyWindow(QQuickItem *parent = nullptr);
    ~BProxyWindow();

    //source
    QString source() const { return m_source; }
    void setSource(const QString &source);

    //proxyVisible
    bool proxyVisible() const;
    void setProxyVisible(bool visible);

    //swap interwal
    int swapInterval() const;
    void setSwapInterval(const int swapInterval);

signals:
    void sourceChanged();
    void swapIntervalChanged();
    void proxyVisibleChanged();

private slots:
    void onWindowChanged(QQuickWindow *window);
    void onAfterSynchronizing();
    void onProcessStarted();
    void onProcessReadyReadStandardOutput();
    void onProcessReadyReadStandardError();

private:
    void startProcess();
    void stopProcess();

    QProcess *m_process;
    QString m_source;
    ProxyWindowController *m_winController;
    QString m_serverName;

    int m_swapInterval;
};
