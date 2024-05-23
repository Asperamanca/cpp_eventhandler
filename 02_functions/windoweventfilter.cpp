#include "windoweventfilter.h"
#include <selectionaccess.h>
#include <lassoaccess.h>
#include <QEvent>
#include <QQuickView>
#include <QQuickItem>
#include <QSinglePointEvent>

CWindowEventFilter::CWindowEventFilter(QObject* pParent)
    : QObject{pParent}
{
}

CWindowEventFilter::~CWindowEventFilter()
{
    setView(nullptr);
}

void CWindowEventFilter::setView(QQuickView* const pView)
{
    if ( m_pView )
    {
        m_pView->removeEventFilter(this);
    }
    m_pView = pView;
    if (m_pView)
    {
        m_pView->installEventFilter(this);
    }
}

void CWindowEventFilter::setEditAllowed(const bool bArg)
{
    m_bEditAllowed = bArg;
}

void CWindowEventFilter::setLassoAllowed(const bool bArg)
{
    m_bLassoAllowed = bArg;
}

bool CWindowEventFilter::eventFilter([[maybe_unused]] QObject* pWatched, QEvent* pEvent)
{
    auto* const pViewRootObject = getViewRootObject();
    if (( ! pEvent) || ( ! pViewRootObject) )
    {
        return false;
    }

    if (auto* const pPointerEvent = dynamic_cast<QSinglePointEvent*>(pEvent))
    {
        auto* const pItemUnderCursor = pViewRootObject->childAt(pPointerEvent->scenePosition().x(),
                                                                pPointerEvent->scenePosition().y());
        //## IMPROVEMENT: Instead of putting all the code in there,
        //  we now call different functions in turn.
        // Notice how easily you can now see which feature gets to handle the
        //  event first. This "priorization" was there before, but much harder to see
        if (handleLassoEvent(*pPointerEvent, pItemUnderCursor))
        {
            return false;
        }
        if (handleDragMoveEvent(*pPointerEvent, pItemUnderCursor))
        {
            return false;
        }
        if (handleSelectionEvent(*pPointerEvent, pItemUnderCursor))
        {
            return false;
        }
    }
    else if (auto* const pKeyEvent = dynamic_cast<QKeyEvent*>(pEvent))
    {
        //## IMPROVEMENT: Same as above, but for key events
        if (handleLassoEvent(*pKeyEvent))
        {
            return true;
        }
        if (handleSelectionEvent(*pKeyEvent))
        {
            return true;
        }
    }
    //## IMPROVEMENT: Function is much easier to read now

    return false;
}

bool CWindowEventFilter::handleLassoEvent(QSinglePointEvent& event, QQuickItem* pItemUnderCursor)
{
    // The code here now only handles the mouse part of the  "lasso selection" feature
    // Those parts of code are pretty much identical to the original version
    switch(event.type())
    {
        case QEvent::MouseButtonPress:
        {
            if ( ( ! pItemUnderCursor) && (m_bLassoAllowed))
            {
                if (auto* const pLassoItem = getLassoItem())
                {
                    m_LassoSelectionOperation = CLassoSelectionOperation(getViewRootObject(),
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
                if (auto* const pLassoItem = getLassoItem())
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
                if (auto* const pLassoItem = getLassoItem())
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

bool CWindowEventFilter::handleLassoEvent(QKeyEvent& event)
{
    // The code here now only handles the keyboard part of the  "lasso selection" feature
    // Those parts of code are pretty much identical to the original version
    if (event.keyCombination() == QKeyCombination(Qt::NoModifier, Qt::Key_Escape))
    {
        m_LassoSelectionOperation.cancel();
        if (auto* const pLassoItem = getLassoItem())
        {
            pLassoItem->setVisible(false);
            return true;
        }
    }
    return false;
}

bool CWindowEventFilter::handleDragMoveEvent(QSinglePointEvent& event, QQuickItem* pItemUnderCursor)
{
    // The code here now only handles the mouse part of the  "drag-move" feature
    // Those parts of code are pretty much identical to the original version
    switch(event.type())
    {
        case QEvent::MouseButtonPress:
        {
            if (event.button() == Qt::LeftButton)
            {
                m_DragStartPos = event.scenePosition();
            }
        }
        case QEvent::MouseMove:
        {
            if (m_bEditAllowed && m_pLeftPressedItem)
            {
                const auto currentPos = event.scenePosition();
                if ( (event.buttons().testFlag(Qt::LeftButton)) &&
                    ( ! m_DragOperation.isValid()) )
                {
                    if (QLineF(m_DragStartPos, currentPos).length() >= sm_ItemDragTreshold)
                    {
                        CSelectionAccess::setSelected(pItemUnderCursor, true);
                        auto selectedItems = CSelectionAccess::getSelectedItemsInTree(getViewRootObject());
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

bool CWindowEventFilter::handleSelectionEvent(QSinglePointEvent& event, QQuickItem* pItemUnderCursor)
{
    // The code here now only handles the mouse part of the  "click selection" feature
    // Those parts of code are pretty much identical to the original version
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
                    CSelectionAccess::setTreeSelected(getViewRootObject(), false);
                    CSelectionAccess::setSelected(pItemUnderCursor, true);
                }
                m_pLeftPressedItem={};
                return true;
            }
            break;
        }
        default: break;
    }
    return false;
}

bool CWindowEventFilter::handleSelectionEvent(QKeyEvent& event)
{
    // The code here now only handles the keyboard part of the  "click selection" feature
    // Those parts of code are pretty much identical to the original version
    if (event.keyCombination() == QKeyCombination(Qt::ControlModifier, Qt::Key_A))
    {
        CSelectionAccess::setTreeSelected(getViewRootObject(), true);
        return true;
    }
    return false;
}

QQuickItem* CWindowEventFilter::getViewRootObject()
{
    if ( ! m_pView)
    {
        return {};
    }
    return m_pView->rootObject();
}

QQuickItem* CWindowEventFilter::getLassoItem()
{
    auto* const pViewRootObject = getViewRootObject();
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
