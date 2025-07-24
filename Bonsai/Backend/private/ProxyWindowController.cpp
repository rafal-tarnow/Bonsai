#include "ProxyWindowController.hpp"

#include <QDebug>
#include <QTimer>

ProxyWindowController::ProxyWindowController(QObject *parent)
    : QObject(parent)
{
    connect(&m_socket, &QLocalSocket::connected, this, &ProxyWindowController::onSocketConnected);
    connect(&m_socket,
            &QLocalSocket::disconnected,
            this,
            &ProxyWindowController::onSocketDisconnected);
    connect(&m_socket, &QLocalSocket::readyRead, this, &ProxyWindowController::onSocketReadyRead);
    connect(&m_socket, &QLocalSocket::errorOccurred, this, &ProxyWindowController::onSocketError);
}

ProxyWindowController::~ProxyWindowController()
{
    if (m_socket.isOpen()) {
        m_socket.flush();
        m_socket.close();
        m_socket.disconnect();
    }
}

bool ProxyWindowController::visible()
{
    return m_proxyWindowVisible;
}

void ProxyWindowController::setVisible(bool visible)
{
    qDebug() << __PRETTY_FUNCTION__;
    if (m_proxyWindowVisible == visible) {
        return;
    }

    sendSetVisible(visible);
}

void ProxyWindowController::quitRemoteProcess()
{
    qDebug() << __PRETTY_FUNCTION__;
    if (m_socket.state() != QLocalSocket::ConnectedState) {
        qWarning() << "[ERROR] " << __PRETTY_FUNCTION__
                   << " Controller: Socket not connected. Cannot send command.";
        return;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint32) 0; //reserve space for frame size
    out << static_cast<quint8>(CommandSend::QUIT_PROCESS);

    out.device()->seek(0);
    out << (quint32) (block.size() - sizeof(quint32));

    m_socket.write(block);
    m_socket.flush();
}

void ProxyWindowController::sendSetVisible(bool visible)
{
    qDebug() << __PRETTY_FUNCTION__;
    if (m_socket.state() != QLocalSocket::ConnectedState) {
        qWarning() << "[ERROR] " << __PRETTY_FUNCTION__
                   << " Controller: Socket not connected. Cannot send command.";
        return;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint32) 0; //reserve space for frame size
    out << static_cast<quint8>(CommandSend::SET_VISIBLE);
    out << visible;

    out.device()->seek(0);
    out << (quint32) (block.size() - sizeof(quint32));

    m_socket.write(block);
    m_socket.flush();
}

void ProxyWindowController::connectToServer(const QString &serverName)
{
    if (m_socket.state() == QLocalSocket::ConnectedState) {
        qWarning() << "[ERROR] " << __PRETTY_FUNCTION__ << " QLocalSocket::ConnectedState";
        return;
    }
    m_serverName = serverName;
    qDebug() << "ProxyWindowController: Attempting to connect to" << m_serverName;
    m_socket.connectToServer(m_serverName);
}

void ProxyWindowController::onSocketConnected()
{
    qDebug("[OK] Connected to Proxy Window Server");
    reconnect_time = 0;
}

void ProxyWindowController::onSocketDisconnected() {}

void ProxyWindowController::onSocketReadyRead()
{
    QDataStream in(&m_socket);

    forever {
        if (m_nextBlockSize == 0) {
            // Jesteśmy na początku nowej ramki, spróbuj odczytać jej rozmiar
            if (m_socket.bytesAvailable() < (int) sizeof(quint32))
                break; // Za mało danych, czekaj na więcej
            in >> m_nextBlockSize;
        }

        if (m_socket.bytesAvailable() < m_nextBlockSize) {
            // Mamy rozmiar, ale cała ramka jeszcze nie przyszła
            break; // Czekaj na resztę danych
        }

        // Mamy całą ramkę, można ją przetworzyć
        handleStateUpdate(in);

        // Zresetuj rozmiar, aby być gotowym na następną ramkę
        m_nextBlockSize = 0;
    }
}

void ProxyWindowController::handleStateUpdate(QDataStream &stream)
{
    //qDebug() << "[INFO] Proxy Window Client, handle state update";
    quint8 updateTypeRaw;
    stream >> updateTypeRaw;
    auto updateType = static_cast<CommandReceived>(updateTypeRaw);

    if (updateType == CommandReceived::SET_VISIBLE) {
        qDebug() << "[INFO] Received visible update frame";
        bool serverVisible;
        stream >> serverVisible;
        //qDebug() << "Controller: Received VisibleChanged, visible:" << serverVisible;

        if (m_proxyWindowVisible != serverVisible) {
            m_proxyWindowVisible = serverVisible;
            emit visibleChanged();
        }
    } else {
        //qWarning() << "Controller: Received unknown state update type:" << updateTypeRaw;
    }
}

void ProxyWindowController::onSocketError(QLocalSocket::LocalSocketError socketError)
{
    // static int errorsCount;
    // errorsCount++;

    qDebug() << "[ERROR] " << __PRETTY_FUNCTION__ << "Local socket error: " << int(socketError)
             << " " << m_socket.errorString() << " socketState: " << int(m_socket.state());

#warning "So, what do we do if the window gets closed?"

    m_socket.close();
    m_socket.abort();

    qDebug() << "[INFO] Proxy Window reconnect time = " << reconnect_time << "ms";
    QTimer::singleShot(reconnect_time++, this, [this]() { connectToServer(m_serverName); });
}
