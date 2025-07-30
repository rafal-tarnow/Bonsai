// #include "BTaskbarModel.hpp"

// BTaskbarModel::BTaskbarModel(QObject *parent) :
//     QSortFilterProxyModel(parent),
//     m_baseModel(this)
// {
//     setSourceModel(&m_baseModel);

//     m_filterTypes[NET::Desktop] = false;
//     m_filterTypes[NET::Notification] = false;
// }

// bool BTaskbarModel::filterDesktop() const
// {
//     return m_filterTypes.value(NET::Desktop, true);
// }

// bool BTaskbarModel::setFilterDesktop(bool enabled)
// {
//     if(m_filterTypes[NET::Desktop] != enabled){
//         m_filterTypes[NET::Desktop] = enabled;
//         invalidateFilter();
//         emit filterDesktopChanged();
//     }
// }

// bool BTaskbarModel::filterNotification() const
// {
//     return m_filterTypes.value(NET::Notification, true);
// }

// bool BTaskbarModel::setFilterNotification(bool enabled)
// {
//     if(m_filterTypes[NET::Notification] != enabled){
//         m_filterTypes[NET::Notification] = enabled;
//         invalidateFilter();
//         emit filterNotificationChanged();
//     }
// }
