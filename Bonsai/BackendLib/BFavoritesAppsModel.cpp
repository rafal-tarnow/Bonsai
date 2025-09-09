#include "BFavoritesAppsModel.hpp"

#include <QDBusPendingReply>

BFavoritesAppsModel::BFavoritesAppsModel(QObject *parent)
{
    //qDebug() << "[STARTUP INFO] " << __PRETTY_FUNCTION__;
    FavApplication::registerDBusTypes();

    m_dbusInterface = new QDBusInterface("org.bonsai.FavoriteApplications",
                                         "/FavoriteApplications",
                                         "org.bonsai.FavoriteApplications",
                                         QDBusConnection::sessionBus(),
                                         this);

    if (!m_dbusInterface->isValid()) {
        qDebug() << "[ERROR] Cannot connect to FavoriteApps D-Bus Service";
    }

    connect(m_dbusInterface,
            SIGNAL(favoriteAdded(const FavApplication &)),
            this,
            SLOT(handleFavoriteAdded(const FavApplication &)));

    connect(m_dbusInterface,
            SIGNAL(favoriteRemoved(const FavApplication &)),
            this,
            SLOT(handleFavoriteRemoved(const FavApplication &)));

    //connect(m_dbusInterface, SIGNAL(favoritesChanged()), this, SLOT(handleFavoritesChanged()));

    // Init - get favorites
    QDBusPendingCall call = m_dbusInterface->asyncCall("getFavorites");
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            this,
            &BFavoritesAppsModel::handleGetFavoritesReply);
}

BFavoritesAppsModel::~BFavoritesAppsModel()
{
    m_dbusInterface->deleteLater();
    m_dbusInterface = nullptr;
}

int BFavoritesAppsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return favApps.size();
}

QVariant BFavoritesAppsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= favApps.size()) {
        return QVariant();
    }

    const FavApplication &app = favApps[index.row()];
    switch (role) {
    case NameRole:
        return app.name;
    case ExecRole:
        return app.exec;
    case IconRole:
        return app.icon;
    case IdRole:
        return app.id;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> BFavoritesAppsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "appName";
    roles[ExecRole] = "appExec";
    roles[IconRole] = "appIcon";
    roles[IdRole] = "appId";
    return roles;
}

void BFavoritesAppsModel::addFavorite(const QString &appId)
{
    qDebug() << "Adding favorite with ID:" << appId << __PRETTY_FUNCTION__;
    if (!m_dbusInterface || !m_dbusInterface->isValid()) {
        qWarning() << "[ERROR] D-Bus interface is not valid";
        return;
    }

    m_dbusInterface->asyncCall("addFavorite", appId);
}

void BFavoritesAppsModel::removeFavorite(const QString &appId)
{
    qDebug() << "Removing favorite with ID:" << appId << __PRETTY_FUNCTION__;
    if (!m_dbusInterface || !m_dbusInterface->isValid()) {
        qWarning() << "[ERROR] D-Bus interface is not valid";
        return;
    }

    m_dbusInterface->asyncCall("removeFavorite", appId);
}

void BFavoritesAppsModel::handleFavoriteAdded(const FavApplication &favApp)
{
    qDebug() << "777777777777777777777" << __PRETTY_FUNCTION__;
    qDebug() << "favApp.id = " << favApp.id;
    qDebug() << "favApp.name = " << favApp.name;
    qDebug() << "favApp.exec = " << favApp.exec;
    qDebug() << "favApp.icon = " << favApp.icon;
    qDebug() << "8888888888888888888888888888888888888888888888888888888888888888888";

    // Check if the app is already in the list to avoid duplicates
    for (int i = 0; i < favApps.size(); ++i) {
        if (favApps[i].id == favApp.id) {
            qDebug() << "Favorite with ID" << favApp.id << "already exists";
            return;
        }
    }

    // Add the new favorite to the model
    beginInsertRows(QModelIndex(), favApps.size(), favApps.size());
    favApps.append(favApp);
    endInsertRows();
}

void BFavoritesAppsModel::handleFavoriteRemoved(const FavApplication &favApp)
{
    qDebug() << "9999999999999999999999" << __PRETTY_FUNCTION__;
    qDebug() << "favApp.id = " << favApp.id;
    qDebug() << "favApp.name = " << favApp.name;
    qDebug() << "favApp.exec = " << favApp.exec;
    qDebug() << "favApp.icon = " << favApp.icon;

    for (int i = 0; i < favApps.size(); ++i) {
        if (favApps[i].id == favApp.id) {
            beginRemoveRows(QModelIndex(), i, i);
            favApps.removeAt(i);
            endRemoveRows();
            return;
        }
    }
}

//void BFavoritesAppsModel::handleFavoritesChanged() {}

void BFavoritesAppsModel::handleGetFavoritesReply(QDBusPendingCallWatcher *watcher)
{
    if (!watcher) {
        qDebug() << "[ERROR] " << __PRETTY_FUNCTION__ << " No valid QDBusPendingCallWatcher found";
        return;
    }

    QDBusPendingReply<QVector<FavApplication>> reply = *watcher;
    if (reply.isError()) {
        qDebug() << "[ERROR] D-Bus call failed:" << reply.error().message();
        watcher->deleteLater();
        return;
    }

    QVector<FavApplication> favorites = reply.value();
    //qDebug() << "Received" << favorites.size() << "favorite applications";

    // for (const FavApplication &app : favorites) {
    //     qDebug() << "-------------------------";
    //     qDebug() << "ID:" << app.id;
    //     qDebug() << "Name:" << app.name;
    //     qDebug() << "Exec:" << app.exec;
    //     qDebug() << "Icon:" << app.icon;
    // }
    //qDebug() << "-------------------------";

    beginResetModel();
    favApps.clear();
    favApps = favorites.toList();
    endResetModel();

    watcher->deleteLater();
}
