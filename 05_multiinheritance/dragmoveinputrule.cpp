#include "dragmoveinputrule.h"
#include <selectionaccess.h>
#include <QSinglePointEvent>
#include <QQuickItem>
#include "inputinfo.h"

CDragMoveInputRule::CDragMoveInputRule()
    : CInputRule(sm_pRuleId)
{
}

bool CDragMoveInputRule::handleSinglePointEvent(const QSinglePointEvent& event, const CInputInfo& inputInfo)
{
    // The implementation here is almost identical. The only change is using
    //  the inputInfo parameter instead of separate parameters
    switch(event.type())
    {
        case QEvent::MouseButtonPress:
        {
            if ((event.button() == Qt::LeftButton) && inputInfo.getItemUnderCursor())
            {
                m_DragStartPos = event.scenePosition();
                m_bDragPossible = true;
            }
        }
        case QEvent::MouseMove:
        {
            if (m_bDragPossible)
            {
                const auto currentPos = event.scenePosition();
                if ( (event.buttons().testFlag(Qt::LeftButton)) &&
                    ( ! m_DragOperation.isValid()) )
                {
                    if (QLineF(m_DragStartPos, currentPos).length() >= sm_ItemDragTreshold)
                    {
                        CSelectionAccess::setSelected(inputInfo.getItemUnderCursor(), true);
                        auto selectedItems = CSelectionAccess::getSelectedItemsInTree(inputInfo.getViewRootObject());
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

