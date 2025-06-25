#include "BTaskbarBaseModel.hpp"

#include <KX11Extras>
#include <kwindowinfo.h>

BTaskbarBaseModel::BTaskbarBaseModel(QObject *parent) : QAbstractListModel(parent)
{
    initWindowList();
    initActiveWindow();

    connect(KX11Extras::self(), &KX11Extras::activeWindowChanged, this, &BTaskbarBaseModel::setActiveWindow);
    connect(KX11Extras::self(), &KX11Extras::windowAdded, this, &BTaskbarBaseModel::addItem);
    connect(KX11Extras::self(), &KX11Extras::windowRemoved, this, &BTaskbarBaseModel::removeItem);

    connect(KX11Extras::self(), &KX11Extras::windowChanged, this, [this](WId id, NET::Properties properties, NET::Properties2 properties2) {

            //qDebug() << __PRETTY_FUNCTION__;

#warning "There is problem with NET::WMVisibleName value, there is difference in value in KF6 and KF5 library version, I dont know how to solve this by now"
        //if (properties & NET::WMWindowType || properties & 0x8000) { //KWindowSystem 5
            //if (properties & NET::WMWindowType || properties & NET::WMVisibleName) { //KWindowSystem 6
            KWindowInfo info(id, NET::WMWindowType | NET::WMVisibleName);
            if (!info.valid()) {
                return;
            }

            //update window type
            if (properties & NET::WMWindowType) {
                NET::WindowType type = info.windowType(NET::AllTypesMask);
                updateWindowType(id, type);
            }

#warning "There is problem with NET::WMVisibleName value, there is difference in value in KF6 and KF5 library version, I dont know how to solve this by now"
            if (properties & 0x8000) { //KWindowSystem 5
                //if (properties & NET::WMVisibleName) { //KWindowSystem 6
                updateWindowName(id, info.visibleName());
            }
        //}
    });
}

BTaskbarBaseModel::~BTaskbarBaseModel()
{

}

int BTaskbarBaseModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

QVariant BTaskbarBaseModel::data(const QModelIndex &index, int role) const
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
    case WindowTypeRole:
        return static_cast<int>(item.windowType);
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> BTaskbarBaseModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[WindowNameRole] = "windowName";
    roles[WindowIconRole] = "windowIcon";
    roles[WindowIdRole] = "windowId";
    roles[WindowActiveRole] = "windowActive";
    roles[WindowTypeRole] = "windowType";
    return roles;
}

void BTaskbarBaseModel::addItem(const WId id)
{
    KWindowInfo info(id, NET::WMWindowType | NET::WMVisibleName);
    if (!info.valid()){
        return;
    }

    NET::WindowType type = info.windowType(NET::AllTypesMask);
    QUrl imageUrl = QUrl(QString("image://backendtaskbaricons/") + QString::number(id));
    addItemImpl(id, info.visibleName(), imageUrl, type);
}

void BTaskbarBaseModel::addItemImpl(const WId id, const QString &text, const QUrl &image, NET::WindowType type)
{
    //check if window exist
    for(const auto &item : m_items){
        if(item.id == id){
            return; //avoid duplicates
        }
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_items.append({id, text, image, false, type});
    endInsertRows();
}

void BTaskbarBaseModel::removeItem(const WId id)
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

void BTaskbarBaseModel::initWindowList()
{
    QList<WId> windows = KX11Extras::windows();
    for (WId wId: windows){
        addItem(wId);
    }
}

void BTaskbarBaseModel::initActiveWindow()
{
    setActiveWindow(KX11Extras::activeWindow());
}

void BTaskbarBaseModel::setActiveWindow(WId newActiveWindow)
{
    for (int i = 0; i < m_items.size(); ++i){
        bool wasActive = m_items[i].windowActive;
        bool shouldBeActive = (m_items[i].id == newActiveWindow);
        if (wasActive != shouldBeActive) {
            m_items[i].windowActive = shouldBeActive;
            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index, {WindowActiveRole});
        }
    }
}

void BTaskbarBaseModel::updateWindowName(WId id, const QString &newName)
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].id == id && m_items[i].text != newName){
            m_items[i].text = newName;
            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index, {WindowNameRole});
            break;
        }
    }
}

void BTaskbarBaseModel::updateWindowType(WId id, NET::WindowType newType)
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].id == id && m_items[i].windowType != newType){
            m_items[i].windowType = newType;
            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index, {WindowTypeRole});
            break;
        }
    }
}
