#pragma once

#include <QObject>
#include <QVariantList>

class FrontendManagerService : public QObject {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bonsai.FrontendManager")

public:
    explicit FrontendManagerService(QObject *parent = nullptr);
    ~ FrontendManagerService();

    //internal Server api
    void activeFrontendChangeConfirmation(const QString &frontendId);

public slots:
    //D-Bus API
    QVariantList getFrontendList();
    QString activeFrontend() const;
    void setActiveFrontend(const QString &frontendId);

signals:
    //D-Bus API
    Q_SCRIPTABLE void frontendAdded(const QString &id,
                                    const QString &name,
                                    const QString &description,
                                    const QString &path);
    Q_SCRIPTABLE void frontendRemoved(const QString &id);
    Q_SCRIPTABLE void activeFrontendChanged(const QString &frontendId);

    //internal Server api
    void activeFrontendChangeRequest(const QString & forntendId);

private:
    struct Frontend{
        QString id;
        QString name;
        QString description;
        QString path;
        bool active;
    };

    QList<Frontend> m_frontends;
    QString m_activeFrontendId;

    void loadFrontends();
    void addFrontend(const Frontend &frontend);
    void removeFrontend(const QString &frontendId);
};
