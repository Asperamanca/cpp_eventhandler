#include "selectioneventhandler.h"
#include <QSinglePointEvent>
#include <QQuickItem>
#include <selectionaccess.h>
#include "inputinfo.h"

bool CSelectionEventHandler::handleSinglePointEvent(const QSinglePointEvent& event, const CInputInfo& inputInfo)
{
    const auto& pItemUnderCursor = inputInfo.getItemUnderCursor();
    switch(event.type())
    {
        case QEvent::MouseButtonPress:
        {
            if (event.button() == Qt::LeftButton)
            {
                m_pLeftPressedItem = pItemUnderCursor;
                return true;
            }
        }
        case QEvent::MouseButtonRelease:
        {
            if (pItemUnderCursor == m_pLeftPressedItem)
            {
                if (event.modifiers().testFlag(Qt::ControlModifier) )
                {
                    CSelectionAccess::toggleSelected(pItemUnderCursor);
                }
                else
                {
                    CSelectionAccess::setTreeSelected(inputInfo.getViewRootObject(), false);
                    CSelectionAccess::setSelected(pItemUnderCursor, true);
                }
                m_pLeftPressedItem = {};
                return true;
            }
            break;
        }
        default: break;
    }
    return false;
}

bool CSelectionEventHandler::handleKeyEvent(const QKeyEvent& event, const CInputInfo& inputInfo)
{
    if (event.keyCombination() == QKeyCombination(Qt::ControlModifier, Qt::Key_A))
    {
        const auto& pViewRootObject = inputInfo.getViewRootObject();
        CSelectionAccess::setTreeSelected(pViewRootObject, true);
        return true;
    }
    return false;
}

