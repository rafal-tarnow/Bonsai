#include "BProxyWindow.hpp"
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTimer>
#include <QUuid>

BProxyWindow::BProxyWindow(QQuickItem *parent)
    : QQuickItem(parent)
    , m_swapInterval(1)
    , m_serverName(QString("/tmp/BProxyWindowServer%1")
                       .arg(QUuid::createUuid().toString(QUuid::WithoutBraces).remove('-')))
{
    qDebug() << "[INFO]" << " Proxy server name: " << m_serverName;
    setFlag(ItemHasContents, false);
    setVisible(false);

    m_process = new QProcess(this);
    m_winController = new ProxyWindowController(this);

    connect(m_process, &QProcess::started, this, &BProxyWindow::onProcessStarted);
    connect(m_process, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error) {
        qWarning() << "BProxyWindow: Failed to start process. Error:" << error;
    });
    connect(m_process,
            &QProcess::readyReadStandardOutput,
            this,
            &BProxyWindow::onProcessReadyReadStandardOutput);
    connect(m_process,
            &QProcess::readyReadStandardError,
            this,
            &BProxyWindow::onProcessReadyReadStandardError);

    connect(m_winController,
            &ProxyWindowController::visibleChanged,
            this,
            &BProxyWindow::proxyVisibleChanged);
    connect(this, &QQuickItem::windowChanged, this, &BProxyWindow::onWindowChanged);
}

BProxyWindow::~BProxyWindow()
{
    qDebug() << __PRETTY_FUNCTION__;
    m_winController->setParent(nullptr);
    m_winController->quitRemoteProcess();
    //m_winController->disconnect();
    //m_winController->deleteLater();

    //stopProcess();
    //m_process->deleteLater();
}

// BProxyWindow::~BProxyWindow()
// {
//     qDebug() << __PRETTY_FUNCTION__ << "Process state:" << m_process->state();

//     // Sprzątamy kontroler
//     qDebug() << "Disconnecting and deleting ProxyWindowController...";
//     m_winController->disconnect();
//     // m_winController->closeConnection();
//     m_winController->deleteLater();
//     m_winController = nullptr;

//     // Jeśli proces nadal działa
//     if (m_process->state() != QProcess::NotRunning) {
//         qDebug() << "Changing parent of QProcess to nullptr...";
//         m_process->setParent(nullptr);

//         qDebug() << "Connecting QProcess::finished to deleteLater...";
//         connect(m_process, &QProcess::finished, m_process, &QObject::deleteLater);

//         // Timer na wymuszenie kill
//         QTimer::singleShot(3000, this, [this]() {
//             if (m_process && m_process->state() != QProcess::NotRunning) {
//                 qWarning() << "Process did not finish in time, killing it.";
//                 m_process->kill();
//             }
//         });

//         qDebug() << "Terminating process...";
//         m_process->terminate();
//     } else {
//         qDebug() << "Process is not running, deleting QProcess...";
//         m_process->deleteLater();
//     }
//     m_process = nullptr;

//     // Usuwamy gniazdo Unixowe
//     qDebug() << "Removing Unix socket:" << m_serverName;
//     QFile::remove(m_serverName);
// }

void BProxyWindow::setSource(const QString &source)
{
    if (m_source != source) {
        m_source = source;
        emit sourceChanged();
    }
}

bool BProxyWindow::proxyVisible() const
{
    qDebug() << "[INFO] " << __PRETTY_FUNCTION__;
    return m_winController->visible();
}

void BProxyWindow::setProxyVisible(bool visible)
{
    qDebug() << "[INFO] " << __PRETTY_FUNCTION__;
    m_winController->setVisible(visible);
}

int BProxyWindow::swapInterval() const
{
    return m_swapInterval;
}

void BProxyWindow::setSwapInterval(const int swapInterval)
{
    if(m_swapInterval != swapInterval){
        m_swapInterval = swapInterval;
        emit swapIntervalChanged();
    }
}

void BProxyWindow::onWindowChanged(QQuickWindow *window)
{
    if (window) {
        connect(window,
                &QQuickWindow::afterSynchronizing,
                this,
                &BProxyWindow::onAfterSynchronizing,
                Qt::UniqueConnection);
    }
}

void BProxyWindow::onAfterSynchronizing()
{
    startProcess(); //we start windows after synchornization becaouse after synchronization window geomerty is calculated
    disconnect(window(),
               &QQuickWindow::afterSynchronizing,
               this,
               &BProxyWindow::onAfterSynchronizing); // Odłączamy, aby uniknąć wielokrotnych wywołań
}

void BProxyWindow::startProcess()
{
    if (m_process->state() == QProcess::Running) {
        return;
    }

    if (m_source.isEmpty()) {
        return;
    }

    QString program = QCoreApplication::applicationFilePath();

    QStringList args = {"--mode",
                        "client",
                        "--source",
                        m_source,
                        "--proxy-window-addr",
                        m_serverName,
                        "--x",
                        QString::number(x()),
                        "--y",
                        QString::number(y()),
                        "--width",
                        QString::number(width()),
                        "--height",
                        QString::number(height()),
                        "--swap-interval",
                        QString::number(swapInterval())};

    m_process->start(program, args);
}

void BProxyWindow::onProcessStarted()
{
    qDebug() << "[INFO] " << __PRETTY_FUNCTION__ << " Try connect to Proxy Window Server";
    m_winController->connectToServer(m_serverName);
}

void BProxyWindow::onProcessReadyReadStandardOutput()
{
    // Odczytaj wszystko, co jest dostępne na standardowym wyjściu
    const QByteArray data = m_process->readAllStandardOutput();
    // Wypisz to w konsoli głównego procesu z prefiksem, aby było wiadomo, skąd pochodzi
    // Używamy noquote() aby uniknąć dodatkowych cudzysłowów i trimmed() aby usunąć białe znaki z końca
    //qDebug().noquote() << "[REMOTE STDOUT]" << QString::fromLocal8Bit(data).trimmed();
}

void BProxyWindow::onProcessReadyReadStandardError()
{
    // Odczytaj wszystko, co jest dostępne na standardowym błędzie
    const QByteArray data = m_process->readAllStandardError();
    // Wypisz to jako ostrzeżenie w konsoli głównego procesu z odpowiednim prefiksem
    //qWarning().noquote() << "[REMOTE STDERR]" << QString::fromLocal8Bit(data).trimmed();
}

void BProxyWindow::stopProcess()
{
    qDebug() << __PRETTY_FUNCTION__;
    if (m_process->state() != QProcess::NotRunning) {
        m_process->terminate();
        qDebug() << "m_process->waitForFinished();";
        m_process->waitForFinished();
    }
}
