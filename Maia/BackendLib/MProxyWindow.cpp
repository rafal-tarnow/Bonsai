#include "MProxyWindow.hpp"
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QQmlContext>
#include <QQmlEngine>
#include <QTimer>
#include <QUuid>
#include <QVariant>

MProxyWindow::MProxyWindow(QQuickItem *parent)
    : QQuickItem(parent)
    , m_swapInterval(1)
    , m_serverName(QString("/tmp/MProxyWindowServer%1")
                       .arg(QUuid::createUuid().toString(QUuid::WithoutBraces).remove('-')))
{
    //qDebug() << "111111111111111111111111111111111111111111111 ---- " << __PRETTY_FUNCTION__;
    setFlag(ItemHasContents, false);
    setVisible(false);

    m_process = new QProcess(this);
    m_winController = new ProxyWindowController(this);

    connect(m_process, &QProcess::started, this, &MProxyWindow::onProcessStarted);
    connect(m_process, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error) {
        qWarning() << "MProxyWindow: Failed to start process. Error:" << error;
    });
    connect(m_process,
            &QProcess::readyReadStandardOutput,
            this,
            &MProxyWindow::onProcessReadyReadStandardOutput);
    connect(m_process,
            &QProcess::readyReadStandardError,
            this,
            &MProxyWindow::onProcessReadyReadStandardError);

    connect(m_winController,
            &ProxyWindowController::visibleReceived,
            this,
            &MProxyWindow::handleVisibleReceived);

    connect(m_winController,
            &ProxyWindowController::proxyWindowConnected,
            this,
            &MProxyWindow::handleProxyWindowConnected);

    connect(this, &QQuickItem::windowChanged, this, &MProxyWindow::onWindowChanged);
}

MProxyWindow::~MProxyWindow()
{
    //qDebug() << __PRETTY_FUNCTION__ << "Process state:" << m_process->state();

    // Sprzątamy kontroler
    //qDebug() << "Disconnecting and deleting ProxyWindowController...";
    m_winController->disconnect();
    m_winController->deleteLater();
    m_winController = nullptr;

    m_process->setParent(nullptr);
#warning "//TODO What if the process doesen't close?"
    connect(m_process, &QProcess::finished, m_process, &QObject::deleteLater);
    stopProcess();

    // Usuwamy gniazdo Unixowe
    qDebug() << "Removing Unix socket:" << m_serverName;
    QFile::remove(m_serverName);
}

void MProxyWindow::setSource(const QUrl &source)
{
    //qDebug() << "4444444444444444444" << source;

    QUrl finalSource = source;

    if (finalSource.isRelative() && !finalSource.isEmpty()) {
        // Poprawne użycie metody statycznej. Nie potrzebujemy wskaźnika na silnik.
        QQmlContext *context = QQmlEngine::contextForObject(this);

        if (context) {
            // Używamy kontekstu do rozwiązania względnego URL.
            finalSource = context->resolvedUrl(source);
        } else {
            // Ten błąd może się pojawić, jeśli obiekt nie został utworzony przez silnik QML
            // (np. został stworzony ręcznie w C++ i nie jest powiązany z kontekstem).
            qWarning() << "MProxyWindow: Could not get QML context for object. Cannot resolve "
                          "relative URL:"
                       << source;
        }
    }

    if (m_sourceUrl != finalSource) {
        m_sourceUrl = finalSource;
        emit sourceChanged();
    }
}

bool MProxyWindow::proxyVisible() const
{
    //qDebug() << "[INFO] " << __PRETTY_FUNCTION__;
    return m_proxyWindowVisible;
}

void MProxyWindow::setProxyVisible(bool visible)
{
    //qDebug() << "66666666666666666 " << __PRETTY_FUNCTION__ << " visible=" << visible;
    if (m_proxyWindowVisible != visible) {
        m_proxyWindowVisible = visible;
        m_winController->setVisible(m_proxyWindowVisible);
        emit proxyVisibleChanged();
    }
}

int MProxyWindow::swapInterval() const
{
    return m_swapInterval;
}

void MProxyWindow::setSwapInterval(const int swapInterval)
{
    if (m_swapInterval != swapInterval) {
        m_swapInterval = swapInterval;
        emit swapIntervalChanged();
    }
}

void MProxyWindow::onWindowChanged(QQuickWindow *window)
{
    if (window) {
        connect(window,
                &QQuickWindow::afterSynchronizing,
                this,
                &MProxyWindow::onAfterSynchronizing,
                Qt::UniqueConnection);
    }
}

void MProxyWindow::onAfterSynchronizing()
{
    //qDebug() << "222222222222222" << __PRETTY_FUNCTION__;
    startProcess(); //we start windows after synchornization becaouse after synchronization window geomerty is calculated
    disconnect(window(),
               &QQuickWindow::afterSynchronizing,
               this,
               &MProxyWindow::onAfterSynchronizing); // Odłączamy, aby uniknąć wielokrotnych wywołań
}

void MProxyWindow::startProcess()
{
    //qDebug() << "33333333333333333333333333" << __PRETTY_FUNCTION__;
    if (m_process->state() == QProcess::Running) {
        return;
    }

    if (m_sourceUrl.isEmpty()) {
        return;
    }

    QString program = QCoreApplication::applicationFilePath();

    QStringList args = {"--mode",
                        "client",
                        "--source",
                        m_sourceUrl.toString(),
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
                        QString::number(swapInterval()),
                        "--proxy-visible",
                        QVariant(m_proxyWindowVisible).toString()};

    //qDebug() << "444444444444444 " << QVariant(m_proxyWindowVisible).toString();
    //qDebug() << "555555555555555 m_proxyWindowVisible = " << m_proxyWindowVisible;

    m_process->start(program, args);
}

void MProxyWindow::onProcessStarted()
{
    //qDebug() << "[INFO] " << __PRETTY_FUNCTION__ << " Try connect to Proxy Window Server";
    m_winController->connectToServer(m_serverName);
}

void MProxyWindow::onProcessReadyReadStandardOutput()
{
    // Odczytaj wszystko, co jest dostępne na standardowym wyjściu
    const QByteArray data = m_process->readAllStandardOutput();
    // Wypisz to w konsoli głównego procesu z prefiksem, aby było wiadomo, skąd pochodzi
    // Używamy noquote() aby uniknąć dodatkowych cudzysłowów i trimmed() aby usunąć białe znaki z końca
    //qDebug().noquote() << "[REMOTE STDOUT]" << QString::fromLocal8Bit(data).trimmed();
}

void MProxyWindow::onProcessReadyReadStandardError()
{
    // Odczytaj wszystko, co jest dostępne na standardowym błędzie
    const QByteArray data = m_process->readAllStandardError();
    // Wypisz to jako ostrzeżenie w konsoli głównego procesu z odpowiednim prefiksem
    //qDebug().noquote() << "[REMOTE STDERR]" << QString::fromLocal8Bit(data).trimmed();
}

void MProxyWindow::handleVisibleReceived(bool visible)
{
    //qDebug() << "7777777777777" << __PRETTY_FUNCTION__ << "visible=" << visible;
    if (m_proxyWindowVisible != visible) {
        m_proxyWindowVisible = visible;
        emit proxyVisibleChanged();
    }
}

void MProxyWindow::handleProxyWindowConnected()
{
    m_winController->setVisible(m_proxyWindowVisible);
}

void MProxyWindow::stopProcess()
{
    //qDebug() << __PRETTY_FUNCTION__;
    if (m_process->state() != QProcess::NotRunning) {
        m_process->terminate();
    }
}
