#include <QQuickItem>
#include <QQuickWindow>
#include <QRegion>
#include <QList>
#include <QSet>
#include <QTimer>
#include <QElapsedTimer>

#include "./private/MaskTracker.hpp"

class MaskedItem : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit MaskedItem(QQuickItem *parent = nullptr);
    ~MaskedItem();

protected:
    void itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData &value) override;
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

public:
    MaskTracker *m_maskTracker;
};




