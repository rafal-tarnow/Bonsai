#include "Model.hpp"

#include <KX11Extras>
#include <kwindowinfo.h>

#include <X11/Xatom.h>
#include <X11/Xdefs.h>

#include <QFile>
#include <QTimer>

TaskbarIconsProvider::TaskbarIconsProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

TaskbarIconsProvider::~TaskbarIconsProvider(){

}

QPixmap TaskbarIconsProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    //CONVER QString id to WId
    bool ok;
    WId win = static_cast<WId>(id.toULongLong(&ok)); // lub toUInt() dla 32-bitowej platformy
    if (!ok) {
        return QPixmap();
    }

    //CHECK REQUESTET SIZE
    QSize actualSize = requestedSize.isValid() ? requestedSize : QSize(16, 16);

    //GET PIXMAP
    QPixmap pixmap = KX11Extras::icon(win, actualSize.width(), actualSize.height());
    if (size) {
        *size = pixmap.size();
    }
    return pixmap;
}

QString TaskbarIconsProvider::getProcessNameByPid(qint64 pid) {
    QString filePath = QString("/proc/%1/comm").arg(pid);
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString processName = in.readLine().trimmed();
        file.close();
        return processName;
    } else {
        qWarning() << "Nie udało się otworzyć pliku:" << filePath;
        return QString();
    }
}

MyListModel::MyListModel(QObject *parent)
    : QAbstractListModel(parent){
    initWindowList();
    initActiveWindow();

    connect(KX11Extras::self(), &KX11Extras::activeWindowChanged, this, [this](WId id) {
        setActiveWindow(id);
    });

    connect(KX11Extras::self(), &KX11Extras::windowAdded, this, [this](WId id) {
        this->addItem(id);
    });

    connect(KX11Extras::self(), &KX11Extras::windowRemoved, this, [this](WId id) {
        this->removeItem(id);
    });
}

void MyListModel::addItem(const WId id){
    KWindowInfo info(id, NET::WMName | NET::WMVisibleName, NET::WM2DesktopFileName | NET::WM2WindowClass | NET::WM2WindowRole);
    if(!info.valid()){
        return;
    }
    qDebug() << "INFOOOOOOOOOOOOOOOOOO: ";
    qDebug() << "                       "  << info.desktopFileName();
    qDebug() << "                       "  << info.windowClassClass();
    qDebug() << "                       "  << info.windowClassName();
    qDebug() << "                       "  << info.windowRole();
    QUrl imageUrl = QUrl(QString("image://backendtaskbaricons/") + QString::number(id));
    addItem(id, /*info.name() + */info.visibleName(), imageUrl);
}

void MyListModel::addItem(const WId id, const QString &text, const QUrl &image) {

    if (text == "Bonsai") {
#warning "WARNING TODO: thic code must be changed to correct way"
#warning "TODO make this condidion more generic"
        KX11Extras::setType(id, NET::WindowType::Desktop);
        return;
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_items.append({id, text, image});
    endInsertRows();
}

void MyListModel::removeItem(const WId id) {
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].id == id) {
            beginRemoveRows(QModelIndex(), i, i);
            m_items.removeAt(i);
            endRemoveRows();
            return;
        }
    }
}


int MyListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_items.count();
}

QVariant MyListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_items.count())
        return QVariant();

    const ListItem &item = m_items[index.row()];

    switch (role) {
    case WindowNameRole:
        return item.text;
    case WindowIconRole:
        return item.imageSource;
    case WindowIdRole:
        return item.id;
    case WindowActiveRole:
        return item.windowActive;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> MyListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[WindowNameRole] = "windowName";
    roles[WindowIconRole] = "windowIcon";
    roles[WindowIdRole] = "windowId";
    roles[WindowActiveRole] = "windowActive";
    return roles;
}

void MyListModel::initWindowList()
{
    QList<WId> windows = KX11Extras::windows();
    WId wId;
    for (auto it = windows.cbegin(), end = windows.cend(); it != end; ++it) {
        wId = (*it);
        addItem(wId);
    }
}

void MyListModel::initActiveWindow()
{
    WId activeWindow = KX11Extras::activeWindow();
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].id == activeWindow) {
            m_items[i].windowActive = true;
            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index, { WindowActiveRole });
            break;
        }else{
            m_items[i].windowActive = false;
        }
    }
}

void MyListModel::setActiveWindow(WId newActiveWindow)
{
    for (int i = 0; i < m_items.size(); ++i) {
        bool wasActive = m_items[i].windowActive;
        bool shouldBeActive = (m_items[i].id == newActiveWindow);

        if (wasActive != shouldBeActive) {
            m_items[i].windowActive = shouldBeActive;
            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index, { WindowActiveRole });
        }
    }
}


