#include "lassoselectionoperation.h"
#include "selectionaccess.h"

CLassoSelectionOperation::CLassoSelectionOperation(QQuickItem* const pRootItem,
                                                   QPointF selectionStartPos)
    : m_pRootItem(pRootItem),
      m_StartPos(selectionStartPos),
      m_CurrentPos(selectionStartPos)
{
    CSelectionAccess::setTreeSelected(m_pRootItem, false);
}

bool CLassoSelectionOperation::isValid() const
{
    return m_pRootItem;
}

QPointF CLassoSelectionOperation::getStartPos() const
{
    return m_StartPos;
}

void CLassoSelectionOperation::updateSelection(const QPointF& currentPos)
{
    if (m_CurrentPos == currentPos)
    {
        return;
    }

    m_CurrentPos = currentPos;
    const auto selectionGeometry = QRectF(m_StartPos,currentPos).normalized();
    // This is not very performant, especially given the simple implementation
    //  in CSelectionAccess. However, the idea of "deselct all" and then
    //  "select items in rectangle" is probably sufficient for most implementations,
    //  provided the functions in CSelectionAccess are optimized
    CSelectionAccess::setTreeSelected(m_pRootItem, false);
    CSelectionAccess::setTreeSelectedByRect(selectionGeometry,m_pRootItem,true);
}

void CLassoSelectionOperation::complete()
{
    *this = {};
}

void CLassoSelectionOperation::cancel()
{
    if ( ! m_pRootItem)
    {
        return;
    }
    CSelectionAccess::setTreeSelected(m_pRootItem, false);
    *this = {};
}
