#pragma once

#include <QQuickImageProvider>
#include <QPixmap>
#include <QAbstractListModel>
#include <QImage>
#include <QUrl>
#include <QFile>

class TaskbarIconsProvider : public QQuickImageProvider
{
public:
    explicit TaskbarIconsProvider();
    ~TaskbarIconsProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
    QString getProcessNameByPid(qint64 pid);
};

class MyListModel : public QAbstractListModel{
    Q_OBJECT

    struct ListItem {
        WId id;
        QString text;
        QUrl imageSource;
        bool windowActive;
    };

public:
    enum ItemRoles {
        WindowNameRole = Qt::UserRole + 1,
        WindowIconRole,
        WindowIdRole,
        WindowActiveRole
    };

    explicit MyListModel(QObject *parent = nullptr);

    void addItem(const WId id);
    void addItem(const WId id, const QString &text, const QUrl &image);
    void removeItem(const WId id);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    void initWindowList();
    void initActiveWindow();
    void setActiveWindow(WId newActiveWindow);
    void updateWindowName(WId id, const QString &newName);

private:
    QList<ListItem> m_items;
};
