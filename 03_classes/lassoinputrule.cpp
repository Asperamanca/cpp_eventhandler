#include "lassoinputrule.h"
#include <lassoaccess.h>
#include <QSinglePointEvent>
#include <QQuickItem>


void CLassoInputRule::setLassoAllowed(const bool bArg)
{
    m_bLassoAllowed = bArg;
}

bool CLassoInputRule::handleLassoEvent(QSinglePointEvent& event, QQuickItem* pItemUnderCursor, QQuickItem* const pViewRootObject)
{
    switch(event.type())
    {
        case QEvent::MouseButtonPress:
        {
            if ( ( ! pItemUnderCursor) && (m_bLassoAllowed))
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

bool CLassoInputRule::handleLassoEvent(QKeyEvent& event, QQuickItem* const pViewRootObject)
{
    if (event.keyCombination() == QKeyCombination(Qt::NoModifier, Qt::Key_Escape))
    {
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

