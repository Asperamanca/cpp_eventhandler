#include "dragmoveinputrule.h"
#include <selectionaccess.h>
#include <QSinglePointEvent>
#include <QQuickItem>


void CDragMoveInputRule::setEditAllowed(const bool bArg)
{
    m_bEditAllowed = bArg;
}

bool CDragMoveInputRule::handleDragMoveEvent(QSinglePointEvent& event, QQuickItem* pItemUnderCursor, QQuickItem* const pViewRootObject)
{
    switch(event.type())
    {
        case QEvent::MouseButtonPress:
        {
            if ((event.button() == Qt::LeftButton) && pItemUnderCursor)
            {
                m_DragStartPos = event.scenePosition();
                m_bDragPossible = true;
            }
        }
        case QEvent::MouseMove:
        {
            if (m_bEditAllowed && m_bDragPossible)
            {
                const auto currentPos = event.scenePosition();
                if ( (event.buttons().testFlag(Qt::LeftButton)) &&
                    ( ! m_DragOperation.isValid()) )
                {
                    if (QLineF(m_DragStartPos, currentPos).length() >= sm_ItemDragTreshold)
                    {
                        CSelectionAccess::setSelected(pItemUnderCursor, true);
                        auto selectedItems = CSelectionAccess::getSelectedItemsInTree(pViewRootObject);
                        m_DragOperation = CItemDragOperation{m_DragStartPos,selectedItems};
                    }
                }
                m_DragOperation.moveItems(currentPos);
                return m_DragOperation.isValid();
            }
            break;
        }
        case QEvent::MouseButtonRelease:
        {
            if (event.button() == Qt::LeftButton)
            {
                m_bDragPossible = false;
            }
            if (m_DragOperation.isValid())
            {
                m_DragOperation={};
                return true;
            }
            break;
        }
        default: break;
    }
    return false;
}

