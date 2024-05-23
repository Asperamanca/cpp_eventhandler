#include "itemdragoperation.h"
#include <ranges>

CItemDragOperation::CItemDragOperation(const QPointF& referencePos, const QList<QQuickItem*>& itemList)
    : m_StartReferencePos(referencePos),
      m_DragData(buildDragDataFromItemList(itemList))
{
}

bool CItemDragOperation::isValid() const
{
    return ( ! m_DragData.isEmpty());
}

void CItemDragOperation::moveItems(const QPointF& newReferencePos)
{
    if (newReferencePos == m_CurrentReferencePos)
    {
        return;
    }

    const auto delta = newReferencePos - m_StartReferencePos;
    m_CurrentReferencePos = newReferencePos;
    updateItemPositions(delta, m_DragData);
}

void CItemDragOperation::updateItemPositions(const QPointF& delta, const QList<CItemDragData>& dragData)
{
    std::ranges::for_each(dragData,[&delta](const CItemDragData& dragData)
                          { dragData.m_pItem->setPosition(dragData.m_ItemStartPos + delta); });
}

QList<CItemDragData> CItemDragOperation::buildDragDataFromItemList(const QList<QQuickItem*>& itemList)
{
    auto view = std::views::filter(itemList,[](QQuickItem* const pItem)->bool
                                    { return pItem;} )
        |       std::views::transform([](QQuickItem* const pItem) -> CItemDragData
                                    { return {.m_pItem=pItem, .m_ItemStartPos=pItem->position()}; });
    return {view.begin(), view.end()};
}
