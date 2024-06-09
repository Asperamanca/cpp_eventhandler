#include "selectioninputrule.h"
#include <QSinglePointEvent>
#include <QQuickItem>
#include <selectionaccess.h>
#include "inputinfo.h"

CSelectionInputRule::CSelectionInputRule()
    : CInputRule(sm_pRuleId)
{
}

bool CSelectionInputRule::handleSinglePointEvent(QSinglePointEvent& event, const CInputInfo& inputInfo)
{
    const auto& pItemUnderCursor = inputInfo.m_pItemUnderCursor;
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
                    CSelectionAccess::setTreeSelected(inputInfo.m_pViewRootObject, false);
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

bool CSelectionInputRule::handleKeyEvent(QKeyEvent& event, const CInputInfo& inputInfo)
{
    if (event.keyCombination() == QKeyCombination(Qt::ControlModifier, Qt::Key_A))
    {
        const auto& pViewRootObject = inputInfo.m_pViewRootObject;
        CSelectionAccess::setTreeSelected(pViewRootObject, true);
        return true;
    }
    return false;
}

