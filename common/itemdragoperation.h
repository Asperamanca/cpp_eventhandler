#ifndef ITEMDRAGOPERATION_H
#define ITEMDRAGOPERATION_H

#include <QQuickItem>
#include <QPointF>
#include <QList>

class CItemDragData
{
public:
    QPointer<QQuickItem> m_pItem{};
    QPointF m_ItemStartPos{};
};

class CItemDragOperation
{
public:
    CItemDragOperation() = default;
    CItemDragOperation(const QPointF& referencePos, const QList<QQuickItem*>& itemList);

    bool isValid() const;
    void moveItems(const QPointF& newReferencePos);

private:
    static void updateItemPositions(const QPointF& delta, const QList<CItemDragData>& dragData);
    static QList<CItemDragData> buildDragDataFromItemList(const QList<QQuickItem*>& itemList);

private:
    QPointF m_StartReferencePos{};
    QPointF m_CurrentReferencePos{};
    QList<CItemDragData> m_DragData{};
};

#endif // ITEMDRAGOPERATION_H
