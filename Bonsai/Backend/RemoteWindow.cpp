#include "RemoteWindow.hpp"
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

RemoteWindow::RemoteWindow(QQuickItem *parent)
    : QQuickItem(parent), m_process(this), m_swapInterval(1)
{
    setFlag(ItemHasContents, false);
    setVisible(false);

    connect(this, &QQuickItem::windowChanged, this, &RemoteWindow::onWindowChanged);
}

RemoteWindow::~RemoteWindow()
{
    stopProcess();
}

void RemoteWindow::setSource(const QString &source)
{
    if (m_source != source) {
        m_source = source;
        emit sourceChanged();
    }
}

int RemoteWindow::swapInterval() const { return m_swapInterval; }

void RemoteWindow::setSwapInterval(const int swapInterval)
{
    if(m_swapInterval != swapInterval){
        m_swapInterval = swapInterval;
        emit swapIntervalChanged();
    }
}

void RemoteWindow::onWindowChanged(QQuickWindow *window) {
    if (window) {
        connect(window, &QQuickWindow::afterSynchronizing, this, &RemoteWindow::onAfterSynchronizing, Qt::UniqueConnection);
    }
}

void RemoteWindow::onAfterSynchronizing() {
    startProcess(); //we start windows after synchornization becaouse after synchronization window geomerty is calculated
    disconnect(window(), &QQuickWindow::afterSynchronizing, this, &RemoteWindow::onAfterSynchronizing); // Odłączamy, aby uniknąć wielokrotnych wywołań
}

void RemoteWindow::startProcess()
{
    if (m_process.state() == QProcess::Running) {
        return;
    }

    if (m_source.isEmpty()) {
        return;
    }

    QString program = QCoreApplication::applicationFilePath();

    QStringList args = { "--mode", "client", "--source", m_source,
                        "--x", QString::number(x()), "--y", QString::number(y()),
                        "--width", QString::number(width()), "--height", QString::number(height()), "--swap-interval", QString::number(swapInterval())};

    m_process.start(program, args);
}

void RemoteWindow::stopProcess()
{
    if (m_process.state() == QProcess::Running) {
        m_process.terminate();
        m_process.waitForFinished();
    }
}
