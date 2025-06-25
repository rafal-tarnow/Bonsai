#pragma once

#include <QQuickItem>
#include <QProcess>
#include <QQuickWindow>

class RemoteWindow : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(int swapInterval READ swapInterval WRITE setSwapInterval NOTIFY swapIntervalChanged)

public:
    explicit RemoteWindow(QQuickItem *parent = nullptr);
    ~RemoteWindow();

    //source
    QString source() const { return m_source; }
    void setSource(const QString &source);

    //swap interwal
    int swapInterval() const;
    void setSwapInterval(const int swapInterval);

signals:
    void sourceChanged();
    void swapIntervalChanged();

private slots:
    void onWindowChanged(QQuickWindow *window);
    void onAfterSynchronizing();

private:
    void startProcess();
    void stopProcess();

    QProcess m_process;
    QString m_source;
    int m_swapInterval;
};
