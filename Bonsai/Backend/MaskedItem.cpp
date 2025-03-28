#include "MaskedItem.hpp"

MaskedItem::MaskedItem(QQuickItem *parent) : QQuickItem(parent)
{
    setFlag(ItemHasContents, false);
    m_maskTracker = new MaskTracker(this, false);
}

MaskedItem::~MaskedItem()
{
    delete m_maskTracker;
}

void MaskedItem::itemChange(ItemChange change, const ItemChangeData &value)
{
    m_maskTracker->itemChange(change, value);
    QQuickItem::itemChange(change, value);
}

void MaskedItem::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    m_maskTracker->geometryChange(newGeometry, oldGeometry);
    QQuickItem::geometryChange(newGeometry, oldGeometry);
}
