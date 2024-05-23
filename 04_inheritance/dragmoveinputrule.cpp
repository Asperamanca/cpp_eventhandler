#include "dragmoveinputrule.h"
#include <selectionaccess.h>
#include <QSinglePointEvent>
#include <QQuickItem>
#include "inputinfo.h"

CDragMoveInputRule::CDragMoveInputRule()
    : CAbstractInputRule(sm_pRuleId)
{
}

bool CDragMoveInputRule::handleSinglePointEvent(QSinglePointEvent& event, const CInputInfo& inputInfo)
{
    // The implementation here is almost identical. The only change is using
    //  the inputInfo parameter instead of separate parameters
    switch(event.type())
    {
        case QEvent::MouseButtonPress:
        {
            if ((event.button() == Qt::LeftButton) && inputInfo.m_pItemUnderCursor)
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
                        CSelectionAccess::setSelected(inputInfo.m_pItemUnderCursor, true);
                        auto selectedItems = CSelectionAccess::getSelectedItemsInTree(inputInfo.m_pViewRootObject);
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

//## ISSUE: What's this? We don't need keyboard events!
// Unfortunately, the interface dictates that we must implement a key event handler, too
//  We don't need it, so we just implement a dummy.
// This does not sound like a big deal, until you get more and more of those. Touch events, Drag & Drop events,
//  Window activation event,... and we always have to write dummies for those we don't want...
bool CDragMoveInputRule::handleKeyEvent([[maybe_unused]] QKeyEvent& event,[[maybe_unused]] const CInputInfo& inputInfo)
{
    return false;
}

