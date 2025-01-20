#include "Model.hpp"

#include <X11/Xatom.h>
#include <X11/Xdefs.h>

#include <QFile>
#include <QTimer>

#include <KX11Extras>
#include <kwindowinfo.h>

QDebug operator<<(QDebug dbg, const KWindowInfo &info) {
    if (!info.valid()) {
        dbg.nospace() << "KWindowInfo(Invalid data)";
        return dbg.space();
    }

    auto windowTypeToString = [](NET::WindowType type) -> QString {
        switch (type) {
        case NET::Unknown: return "Unknown";
        case NET::Normal: return "Normal";
        case NET::Desktop: return "Desktop";
        case NET::Dock: return "Dock";
        case NET::Toolbar: return "Toolbar";
        case NET::Menu: return "Menu";
        case NET::Dialog: return "Dialog";
        case NET::Override: return "Override";
        case NET::TopMenu: return "TopMenu";
        case NET::Utility: return "Utility";
        case NET::Splash: return "Splash";
        case NET::DropdownMenu: return "DropdownMenu";
        case NET::PopupMenu: return "PopupMenu";
        case NET::Tooltip: return "Tooltip";
        case NET::Notification: return "Notification";
        case NET::ComboBox: return "ComboBox";
        case NET::DNDIcon: return "DNDIcon";
        case NET::OnScreenDisplay: return "OnScreenDisplay";
        case NET::CriticalNotification: return "CriticalNotification";
        case NET::AppletPopup: return "AppletPopup";
        default: return "Other";
        }
    };

    dbg.nospace() << "KWindowInfo:\n"
                  << "\tWindow ID: " << info.win() << "\n"
                  << "\tValid: " << info.valid() << "\n"
                  << "\tName: " << info.name() << "\n"
                  << "\tVisible Name: " << info.visibleName() << "\n"
                  << "\tIcon Name: " << info.iconName() << "\n"
                  << "\tDesktop: " << info.desktop() << "\n"
                  << "\tState: " << info.state() << "\n"
                  << "\tMinimized: " << info.isMinimized() << "\n"
                  << "\tOn Current Desktop: " << info.isOnCurrentDesktop() << "\n"
                  << "\tFrame Geometry: " << info.frameGeometry() << "\n"
                  << "\tGeometry: " << info.geometry() << "\n"
                  << "\tClient Machine: " << info.clientMachine() << "\n"
                  << "\tWindow Class Name: " << info.windowClassName() << "\n"
                  << "\tWindow Class Class: " << info.windowClassClass() << "\n"
                  << "\tGTK Application ID: " << info.gtkApplicationId() << "\n"
                  << "\tTransient For: " << info.transientFor() << "\n"
                  << "\tActivities: " << info.activities().join(", ") << "\n"
                  << "\tWindow Type: "
                  << windowTypeToString(info.windowType(NET::AllTypesMask)) << "\n"; // Dodano typ okna

    return dbg.space();
}



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

    connect(KX11Extras::self(), &KX11Extras::windowChanged, this, [this](WId id, NET::Properties properties, NET::Properties2 properties2) {

#warning "There is problem with NET::WMVisibleName value, there is difference in value in KF6 and KF5 library version, I dont know how to solve this by now"
        if (properties & NET::WMWindowType || properties & 0x8000) { //KWindowSystem 5
            //if (properties & NET::WMWindowType || properties & NET::WMVisibleName) { //KWindowSystem 6
            KWindowInfo info(id, NET::WMWindowType | NET::WMVisibleName);
            if (!info.valid()) {
                return;
            }

#warning "What if window was Desktop or Dock and has changed to normal window? "
            if (properties & NET::WMWindowType) {
                NET::WindowType type = info.windowType(NET::AllTypesMask);
                if (type == NET::Desktop || type == NET::Dock) {
                    this->removeItem(id);
                }
            }

            #warning "There is problem with NET::WMVisibleName value, there is difference in value in KF6 and KF5 library version, I dont know how to solve this by now"
            if (properties & 0x8000) { //KWindowSystem 5
            //if (properties & NET::WMVisibleName) { //KWindowSystem 6
                updateWindowName(id, info.visibleName());
            }
        }
    });
}

void MyListModel::addItem(const WId id)
{
    KWindowInfo info(id, NET::WMWindowType | NET::WMVisibleName);
    if (!info.valid()){
        return;
    }
    if((info.windowType(NET::AllTypesMask) != NET::Desktop) && (info.windowType(NET::AllTypesMask) != NET::Dock))
    {
        QUrl imageUrl = QUrl(QString("image://backendtaskbaricons/") + QString::number(id));
        addItem(id, info.visibleName(), imageUrl);
    }
}

void MyListModel::addItem(const WId id, const QString &text, const QUrl &image)
{

    qDebug() << "TaskbarModel::addItem :" << id << ";" << text << ";" << image;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_items.append({id, text, image});
    endInsertRows();
}

void MyListModel::removeItem(const WId id)
{
    for (int i = 0; i < m_items.size(); ++i)
    {
        if (m_items[i].id == id)
        {
            beginRemoveRows(QModelIndex(), i, i);
            m_items.removeAt(i);
            endRemoveRows();
            return;
        }
    }
}

int MyListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

QVariant MyListModel::data(const QModelIndex &index, int role) const
{
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

QHash<int, QByteArray> MyListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[WindowNameRole] = "windowName";
    roles[WindowIconRole] = "windowIcon";
    roles[WindowIdRole] = "windowId";
    roles[WindowActiveRole] = "windowActive";
    return roles;
}

void MyListModel::initWindowList()
{
    qDebug() << __PRETTY_FUNCTION__;
    QList<WId> windows = KX11Extras::windows();
    WId wId;
    for (auto it = windows.cbegin(), end = windows.cend(); it != end; ++it)
    {
        wId = (*it);
        //get window info
        KWindowInfo info(wId, NET::WMWindowType);
        if (!info.valid())
        {
            continue;
        }
        //don't add Desktop and Dock windows to taskbar list
        if((info.windowType(NET::AllTypesMask) != NET::Desktop) && (info.windowType(NET::AllTypesMask) != NET::Dock)){
            addItem(wId);
        }
    }
}

void MyListModel::initActiveWindow()
{
    WId activeWindow = KX11Extras::activeWindow();
    for (int i = 0; i < m_items.size(); ++i)
    {
        if (m_items[i].id == activeWindow)
        {
            m_items[i].windowActive = true;
            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index, { WindowActiveRole });
            break;
        }
        else
        {
            m_items[i].windowActive = false;
        }
    }
}

void MyListModel::setActiveWindow(WId newActiveWindow)
{
    for (int i = 0; i < m_items.size(); ++i)
    {
        bool wasActive = m_items[i].windowActive;
        bool shouldBeActive = (m_items[i].id == newActiveWindow);

        if (wasActive != shouldBeActive) {
            m_items[i].windowActive = shouldBeActive;
            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index, { WindowActiveRole });
        }
    }
}

void MyListModel::updateWindowName(WId id, const QString &newName) {
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].id == id) {
            if (m_items[i].text != newName) {
                m_items[i].text = newName;
                QModelIndex index = createIndex(i, 0);
                emit dataChanged(index, index, { WindowNameRole });
            }
            break;
        }
    }
}



