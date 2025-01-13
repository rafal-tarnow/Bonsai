#pragma once

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QAbstractListModel>
#include <QList>

class KDesktopFile;
class KConfigGroup;


struct Application {
    QString name;
    QString exec;
    QString icon;
    QString id;

    bool operator==(const Application& other) const {
        return id == other.id;
    }
};



class ApplicationModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum ApplicationRoles {
        NameRole = Qt::UserRole + 1,
        ExecRole,
        IconRole,
        IdRole
    };

    ApplicationModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    QString getExecById(const QString& appId) const;

private:
    QList<Application> apps;
};



#include <QSortFilterProxyModel>
#include <QRegularExpression>

class FilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit FilterProxyModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override {
        QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        QString itemText = index.data(ApplicationModel::NameRole).toString();

        // Użyj filterRegExp().pattern() jako wzoru
        return itemText.contains(filterRegularExpression().pattern(), Qt::CaseInsensitive);// && (sourceRow > 1) && (sourceRow < 1000);
    }
};
