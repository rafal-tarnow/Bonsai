#include "BFrontendManager.hpp"
#include <QDir>
#include <QUuid>
#include <QDBusPendingReply>
#include <QVariant>
#include <QVariantList>
#include <qdbusmetatype.h>

BFrontendManager::BFrontendManager(QObject *parent) : QAbstractListModel(parent)
{
    qDBusRegisterMetaType<QVariantMap>();
    qDBusRegisterMetaType<QVariantList>();

    m_dbusInterface = new QDBusInterface("org.bonsai.FrontendManager",
                                         "/FrontendManager",
                                         "org.bonsai.FrontendManager",
                                         QDBusConnection::sessionBus(),
                                         this);

    // Połącz sygnały D-Bus z slotami
    connect(m_dbusInterface,
            SIGNAL(frontendAdded(QString, QString, QString, QString)),
            this,
            SLOT(handleFrontendAdded(QString, QString, QString, QString)));
    connect(m_dbusInterface,
            SIGNAL(frontendRemoved(QString)),
            this,
            SLOT(handleFrontendRemoved(QString)));
    connect(m_dbusInterface,
            SIGNAL(activeFrontendChanged(QString)),
            this,
            SLOT(handleFrontendChanged(QString)));

    // Pobierz początkową listę frontendów
    QDBusPendingCall call = m_dbusInterface->asyncCall("getFrontendList");
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            this,
            &BFrontendManager::handleFrontendListReply);

    // Pobierz początkowy activeFrontendId
    loadActiveFrontend();
}

BFrontendManager::~BFrontendManager()
{
    delete m_dbusInterface;
}

int BFrontendManager::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_frontends.size();
}

QVariant BFrontendManager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_frontends.size())
        return QVariant();

    const Frontend &frontend = m_frontends[index.row()];
    switch (role) {
    case IdRole:
        return frontend.id;
    case NameRole:
        return frontend.name;
    case DescriptionRole:
        return frontend.description;
    case PathRole:
        return frontend.path;
    case ActiveRole:
        return frontend.active;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> BFrontendManager::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "frontendId";
    roles[NameRole] = "frontendName";
    roles[DescriptionRole] = "frontendDescription";
    roles[PathRole] = "frontendPath";
    roles[ActiveRole] = "frontendActive";
    return roles;
}

QString BFrontendManager::activeFrontend() const
{
    return m_activeFrontendIdMirror;
}

void BFrontendManager::setActiveFrontend(const QString &frontendId)
{
    qDebug() << __PRETTY_FUNCTION__ << " Client side, befor send request via d-bus";
    QDBusPendingCall call = m_dbusInterface->asyncCall("setActiveFrontend", frontendId);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [](QDBusPendingCallWatcher *watcher) {
        if (watcher->isError()) {
            qDebug() << "Failed to switch frontend:" << watcher->error().message();
        }
        watcher->deleteLater();
    });
}

void BFrontendManager::handleFrontendListReply(QDBusPendingCallWatcher *watcher)
{
    qDebug() << "A.A--- " << __PRETTY_FUNCTION__;
    QDBusPendingReply<QVariantList> reply = *watcher;
    if (reply.isError()) {
        qDebug() << "D-Bus error:" << reply.error().message();
        watcher->deleteLater();
        return;
    }

    beginResetModel();
    m_frontends.clear();

    QVariantList frontends = reply.value();
    qDebug() << "REPLY = " << frontends;

    for (const QVariant &frontendVar : frontends) {
        // Sprawdź, czy QVariant zawiera QDBusArgument
        if (frontendVar.canConvert<QDBusArgument>()) {
            QDBusArgument dbusArg = frontendVar.value<QDBusArgument>();
            QVariantMap map;

            // Deserializuj QDBusArgument jako QVariantMap
            if (dbusArg.currentSignature() == "a{sv}") {
                dbusArg >> map;
            } else {
                qDebug() << "Unexpected D-Bus argument signature:" << dbusArg.currentSignature();
                continue;
            }

            Frontend frontend;
            frontend.id = map["id"].toString();
            frontend.name = map["name"].toString();
            frontend.description = map["description"].toString();
            frontend.path = map["path"].toString();
            frontend.active = map["active"].toBool();

            //qDebug() << "FRONTEND NAME =" << frontend.name;
            //qDebug() << "FRONTEND ID = " << frontend.id;
            //qDebug() << "FRONTEND DESCRIPTION = " << frontend.description;
            //qDebug() << "FRONTEND PATH = " << frontend.path;
            //qDebug() << "FRONTEND ACTIVE = " << frontend.active;
            m_frontends.append(frontend);
        } else {
            qDebug() << "[ERROR] Unexpected QVariant type:" << frontendVar.typeName();
        }
    }

    endResetModel();
    watcher->deleteLater();
}

void BFrontendManager::handleFrontendAdded(const QString &id,
                                           const QString &name,
                                           const QString &description,
                                           const QString &path)
{
    beginInsertRows(QModelIndex(), m_frontends.size(), m_frontends.size());
    Frontend frontend;
    frontend.id = id;
    frontend.name = name;
    frontend.description = description;
    frontend.path = path;
    frontend.active = (id == m_activeFrontendIdMirror);
    m_frontends.append(frontend);
    endInsertRows();
}

void BFrontendManager::handleFrontendRemoved(const QString &id)
{
    for (int i = 0; i < m_frontends.size(); ++i) {
        if (m_frontends[i].id == id) {
            beginRemoveRows(QModelIndex(), i, i);
            m_frontends.removeAt(i);
            endRemoveRows();
            return;
        }
    }
}

void BFrontendManager::handleFrontendChanged(const QString &frontendId)
{
    qDebug() << __PRETTY_FUNCTION__ << " frontendId = " << frontendId;
    if (m_activeFrontendIdMirror != frontendId) {
        // Update active status for all frontends
        for (int i = 0; i < m_frontends.size(); ++i) {
            bool newActive = m_frontends[i].id == frontendId;
            if (m_frontends[i].active != newActive) {
                m_frontends[i].active = newActive;
                qDebug() << "D--- " << "emit dataChanged()";
                emit dataChanged(index(i), index(i), {ActiveRole});
            }
        }
        m_activeFrontendIdMirror = frontendId;
        emit activeFrontendChanged();
    }
}

void BFrontendManager::loadActiveFrontend()
{
    qDebug() << "A--- " << __PRETTY_FUNCTION__;
    QDBusPendingCall call = m_dbusInterface->asyncCall("activeFrontend");
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this](QDBusPendingCallWatcher *watcher) {
        qDebug() << "B--- " << "active frontend lambda";
        QDBusPendingReply<QString> reply = *watcher;
        if (!reply.isError()) {
            handleFrontendChanged(reply.value());
        } else {
            qDebug() << "Failed to get initial activeFrontendId:" << reply.error().message();
        }
        watcher->deleteLater();
    });
}
