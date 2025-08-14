#include "FavoriteAppsService.hpp"
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>

FavoriteAppsDBus::FavoriteAppsDBus(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    FavApplication::registerDBusTypes();

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if (!sessionBus.registerService("org.bonsai.FavoriteApplications")) {
        qFatal("Failed to register D-Bus service: %s", qPrintable(sessionBus.lastError().message()));
    }

    if (!sessionBus.registerObject("/FavoriteApplications",
                                   this,
                                   QDBusConnection::ExportScriptableSlots)) {
        qFatal("Failed to register D-Bus object: %s", qPrintable(sessionBus.lastError().message()));
    }
}

FavoriteAppsDBus::~FavoriteAppsDBus() {}

void FavoriteAppsDBus::addFavorite(const QString &appId)
{
    qDebug() << __PRETTY_FUNCTION__ << " id=" << appId;
    return;
}

void FavoriteAppsDBus::removeFavorite(const QString &appId)
{
    qDebug() << __PRETTY_FUNCTION__ << " id=" << appId;
    return;
}

QVector<FavApplication> FavoriteAppsDBus::getFavorites()
{
    qDebug() << "TEST 3 TEST 3 TEST 3 TEST 3 TEST 3 TEST 3 TEST 3 D-BUS";

    QVector<FavApplication> favApplications;

    FavApplication app;
    app.name = "app 1";
    app.id = "id 1";

    favApplications.append(app);

    app.name = "app 2";
    app.id = "id 2";

    favApplications.append(app);

    return favApplications;
}

void FavoriteAppsDBus::testSlot()
{
    qDebug() << "TEST 1 TEST 1 TEST 1 TEST 1 TEST 1 TEST 1 TEST 1 D-BUS";
}

void FavoriteAppsDBus::testSlotSecond()
{
    qDebug() << "TEST 2 TEST 2 TEST 2 TEST 2 TEST 2 TEST 2 TEST 2 D-BUS";
}

FavoriteAppsService::FavoriteAppsService(QObject *parent)
    : QObject(parent)
    , favoriteAppsDBus(this)
{}

FavoriteAppsService::~FavoriteAppsService() {}
