#include "selectioninputrule.h"
#include <QSinglePointEvent>
#include <QQuickItem>
#include <selectionaccess.h>


bool CSelectionInputRule::handleSelectionEvent(QSinglePointEvent& event, QQuickItem* pItemUnderCursor, QQuickItem* const pViewRootObject)
{
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
                    CSelectionAccess::setTreeSelected(pViewRootObject, false);
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

bool CSelectionInputRule::handleSelectionEvent(QKeyEvent& event, QQuickItem* const pViewRootObject)
{
    if (event.keyCombination() == QKeyCombination(Qt::ControlModifier, Qt::Key_A))
    {
        CSelectionAccess::setTreeSelected(pViewRootObject, true);
        return true;
    }
    return false;
}
