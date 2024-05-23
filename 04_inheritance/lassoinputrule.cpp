#include "lassoinputrule.h"
#include <lassoaccess.h>
#include "inputinfo.h"
#include <QSinglePointEvent>
#include <QQuickItem>


CLassoInputRule::CLassoInputRule()
    : CAbstractInputRule(sm_pRuleId)
{
}


bool CLassoInputRule::handleSinglePointEvent(QSinglePointEvent& event, const CInputInfo& inputInfo)
{
    const auto& pViewRootObject = inputInfo.m_pViewRootObject;
    switch(event.type())
    {
        case QEvent::MouseButtonPress:
        {
            if ( ! inputInfo.m_pItemUnderCursor)
            {
                if (auto* const pLassoItem = getLassoItem(pViewRootObject))
                {
                    m_LassoSelectionOperation = CLassoSelectionOperation(pViewRootObject,
                                                                         event.scenePosition());
                    return true;
                }
            }
            break;
        }
        case QEvent::MouseMove:
        {
            if (m_LassoSelectionOperation.isValid())
            {
                const auto currentPos = event.scenePosition();
                if (auto* const pLassoItem = getLassoItem(pViewRootObject))
                {
                    const auto lassoGeometry = QRectF(m_LassoSelectionOperation.getStartPos(),currentPos).normalized();
                    pLassoItem->setPosition(lassoGeometry.topLeft());
                    pLassoItem->setSize(lassoGeometry.size());
                    pLassoItem->setVisible(true);
                    m_LassoSelectionOperation.updateSelection(currentPos);
                    return true;
                }
            }
            break;
        }
        case QEvent::MouseButtonRelease:
        {
            if ((event.button() == Qt::LeftButton) && m_LassoSelectionOperation.isValid())
            {
                m_LassoSelectionOperation.complete();
                if (auto* const pLassoItem = getLassoItem(pViewRootObject))
                {
                    pLassoItem->setVisible(false);
                }
                return true;
            }
            break;
        }
        default: break;
    }
    return false;
}

bool CLassoInputRule::handleKeyEvent(QKeyEvent& event, const CInputInfo& inputInfo)
{
    if (event.keyCombination() == QKeyCombination(Qt::NoModifier, Qt::Key_Escape))
    {
        const auto& pViewRootObject = inputInfo.m_pViewRootObject;
        m_LassoSelectionOperation.cancel();
        if (auto* const pLassoItem = getLassoItem(pViewRootObject))
        {
            pLassoItem->setVisible(false);
            return true;
        }
    }
    return false;
}

QQuickItem* CLassoInputRule::getLassoItem(QQuickItem* const pViewRootObject)
{
    if ( ! pViewRootObject)
    {
        return{};
    }
    if ( ! m_pLasso)
    {
        m_pLasso = CLassoAccess::getLassoItem(pViewRootObject);
    }
    return m_pLasso;
}

