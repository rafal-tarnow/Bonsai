#pragma once

#include <QDBusAbstractAdaptor>
#include <QObject>
#include <QVector>

#include "Application.hpp"

class FavoriteAppsDBus : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bonsai.FavoriteApplications")

public:
    explicit FavoriteAppsDBus(QObject *parent = nullptr);
    ~FavoriteAppsDBus();

public slots:
    Q_SCRIPTABLE Q_NOREPLY void addFavorite(const QString &appId);
    Q_SCRIPTABLE Q_NOREPLY void removeFavorite(const QString &appId);
    Q_SCRIPTABLE QVector<FavApplication> getFavorites();
    Q_SCRIPTABLE Q_NOREPLY void testSlot();
    Q_SCRIPTABLE Q_NOREPLY void testSlotSecond();
};

class FavoriteAppsService : public QObject
{
    Q_OBJECT
public:
    explicit FavoriteAppsService(QObject *parent = nullptr);
    ~FavoriteAppsService();

private:
    FavoriteAppsDBus favoriteAppsDBus;
};
