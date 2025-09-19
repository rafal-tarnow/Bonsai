#include "BMaskedItem.hpp"

BMaskedItem::BMaskedItem(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, false);
    m_maskTracker = new MaskTracker(this, false);
}

BMaskedItem::~BMaskedItem()
{
    delete m_maskTracker;
}

void BMaskedItem::itemChange(ItemChange change, const ItemChangeData &value)
{
    m_maskTracker->itemChange(change, value);
    QQuickItem::itemChange(change, value);
}

void BMaskedItem::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    m_maskTracker->geometryChange(newGeometry, oldGeometry);
    QQuickItem::geometryChange(newGeometry, oldGeometry);
}
