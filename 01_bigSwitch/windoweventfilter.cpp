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
    // This will remove the event filter (see below)
    setView(nullptr);
}

void CWindowEventFilter::setView(QQuickView* const pView)
{
    if (m_pView)
    {
        // Removes the event filter from any currently set view,
        //  before we change it
        m_pView->removeEventFilter(this);
        // Needs to be reset
        // (see getLassoItem() to understand why, but it's not very important)
        m_pLasso = nullptr;
    }
    m_pView = pView;
    if (m_pView)
    {
        // Install an event filter on the window, so we receive all events
        //  (including the input events we are interested in)
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

// We don't need the parameter "pWatched".
//  The attribute [[maybe_unused]] suppressed any possible warnings about not using a paramter
bool CWindowEventFilter::eventFilter([[maybe_unused]] QObject* pWatched, QEvent* pEvent)
{
    // We may want to do something with some GUI object, so let's
    //  get the root of the tree. From there, we can find any GUI object.
    auto* const pViewRootObject = getViewRootObject();
    // Some sanity checks (should actually never fail)
    if (( ! pEvent) || ( ! pViewRootObject) )
    {
        return false;
    }

    // QT: Qt provides the type of event in form of an enumeration
    const auto eEventType = pEvent->type();

    // All events that work on a single pointer device (such as a mouse) can be
    //  dynamically cast to QSinglePointEvent, which will provide useful information such
    //  as position of the cursor, or button press states
    // If the cast fails, it's a different type of event
    if (auto* const pPointerEvent = dynamic_cast<QSinglePointEvent*>(pEvent))
    {
        // I can now detect which (visual) item is under the cursor (or nullptr for none)
        //  Note: The background is not considered an item.
        auto* const pItemUnderCursor = pViewRootObject->childAt(pPointerEvent->scenePosition().x(),
                                                                pPointerEvent->scenePosition().y());
        switch(eEventType)
        {
            // Note that mouse button events are divided into "Press" (button goes 'down')
            //  and "Release" (button goes 'up')
            // So don't mix up a "Press" event with "clicking a button" - it's just the 'down' part.
            case QEvent::MouseButtonPress:
            {
                if (pPointerEvent->button() == Qt::LeftButton)
                {
                    // If the left button has been pressed, remember on which item that happened (or nullptr)
                    m_pLeftPressedItem = pItemUnderCursor;
                    // In case this becomes a drag operation, remember the start position
                    m_DragStartPos = pPointerEvent->scenePosition();
                    // However, if I am not on an item, I may be allowed to start a lasso operation
                    if ( ( ! pItemUnderCursor) && (m_bLassoAllowed))
                    {
                        // I need to be able to find the visual item that will draw the lasso
                        // (should never fail)
                        if (auto* const pLassoItem = getLassoItem())
                        {
                            // Start a lasso selection operation
                            m_LassoSelectionOperation = CLassoSelectionOperation(pViewRootObject,
                                                                                 m_DragStartPos);
                        }
                    }
                }
                break;
            } // End MouseButtonPress
            case QEvent::MouseMove:
            {
                const auto currentPos = pPointerEvent->scenePosition();
                // If I have a running lasso selection operation, I need to update it
                if (m_LassoSelectionOperation.isValid())
                {
                    if (auto* const pLassoItem = getLassoItem())
                    {
                        // Span lasso between start and current positions
                        // QT: QRectF is an unrotated rectangle using 'double' for coordinates
                        //  (or 'float' on some embedded systems)
                        //  It's convenient because it provides basic geometry calculations for rectangles
                        const auto lassoGeometry = QRectF(m_DragStartPos,currentPos).normalized();
                        pLassoItem->setPosition(lassoGeometry.topLeft());
                        pLassoItem->setSize(lassoGeometry.size());
                        pLassoItem->setVisible(true);
                        // Update item selection based on current lasso
                        m_LassoSelectionOperation.updateSelection(currentPos);
                    }
                }
                // Otherwise, I might need to start and/or update a drag-move operation,
                //  but only if allowed and if we started the left click on an item
                else if ((m_bEditAllowed) && (m_pLeftPressedItem))
                {
                    // Start an operation only if left button is still down, and we haven't starte one yet
                    // QT: buttons() returns a kind of bitfield called QFlags.
                    //  Bits can be queried separately or in combination using variuos 'test' functions
                    //  Bitwise operations are also provided
                    //  Here we just want to know whether the "LeftButton"-bit is set
                    if ( (pPointerEvent->buttons().testFlag(Qt::LeftButton)) &&
                        ( ! m_DragOperation.isValid()) )
                    {
                        // Start an operation only if we already exceeded the treshold
                        // QT: QLineF represents a line using 'double' (or float) coordinates
                        //  Like QRectF before, it offers some basic geometry calculations, such
                        //  as the length of the line based on start and end point
                        if (QLineF(m_DragStartPos, currentPos).length() >= sm_ItemDragTreshold)
                        {
                            // Immediately select the item under cursor
                            CSelectionAccess::setSelected(pItemUnderCursor, true);
                            // Get all selected items
                            auto selectedItems = CSelectionAccess::getSelectedItemsInTree(pViewRootObject);
                            // Start a drag-move operation on all selected items
                            m_DragOperation = CItemDragOperation{m_DragStartPos,selectedItems};
                        }
                    }
                    // In any case, update the drag operation (will ignore if none running)
                    m_DragOperation.moveItems(currentPos);
                }
            } // End MouseMove
            case QEvent::MouseButtonRelease:
            {
                // Check if left button was released
                if (pPointerEvent->button() == Qt::LeftButton)
                {
                    // Check if lasso selection operation was running
                    if (m_LassoSelectionOperation.isValid())
                    {
                        // End lasso selection operation
                        m_LassoSelectionOperation.complete();
                        if (auto* const pLassoItem = getLassoItem())
                        {
                            // Hide visual lasso item
                            pLassoItem->setVisible(false);
                        }
                    }
                    // Check if drag-move operation was running
                    else if (m_DragOperation.isValid())
                    {
                        // End drag-move operation
                        m_DragOperation={};
                    }
                    // Check if if the item now under the cursor matches the initially left-pressed item
                    else if (pItemUnderCursor == m_pLeftPressedItem)
                    {
                        // modifiers() returns QFlags for the keyboard modifiers
                        //  (Shift, Control, Alt and some others)
                        if (pPointerEvent->modifiers().testFlag(Qt::ControlModifier) )
                        {
                            // With control pressed, we want to toggle selection
                            CSelectionAccess::toggleSelected(pItemUnderCursor);
                        }
                        else
                        {
                            // Without control pressed, we first deselect all...
                            CSelectionAccess::setTreeSelected(pViewRootObject, false);
                            // ...then select the item under cursor
                            // This could be nullptr, in which case we only deselect
                            CSelectionAccess::setSelected(pItemUnderCursor, true);
                        }
                    }
                    // End of left button press-release sequence
                    m_pLeftPressedItem = nullptr;
                }
                break;
            } // End MouseButtonRelease
            default: break;
        }
    }
    // Check if we received a key event
    else if (auto* const pKeyEvent = dynamic_cast<QKeyEvent*>(pEvent))
    {
        switch(pKeyEvent->type())
        {
            case QEvent::KeyPress: break;
            // Only respond to key release
            case QEvent::KeyRelease:
            {
                // If Escape was pressed (without modifiers)...
                if ((pKeyEvent->keyCombination() == QKeyCombination(Qt::NoModifier, Qt::Key_Escape)) &&
                    (m_LassoSelectionOperation.isValid()))
                {
                    // Cancel any possibly running lasso selection
                    m_LassoSelectionOperation.cancel();
                    if (auto* const pLassoItem = getLassoItem())
                    {
                        // Hide visual lasso item
                        pLassoItem->setVisible(false);
                    }
                    // I handled the key - stop the event from further propagation
                    return true;
                }
                // If Control-A was pressed...
                if (pKeyEvent->keyCombination() == QKeyCombination(Qt::ControlModifier, Qt::Key_A))
                {
                    // Select all
                    CSelectionAccess::setTreeSelected(getViewRootObject(), true);
                    // I handled the key - stop the event from further propagation
                    return true;
                }
            }
            default: break;
        }
    }

    // Pass on the event for further handling
    return false;
}
//## ISSUE: Long function
//  This function is already pretty long, and this is only going to get worse
//  as we add more and more event handlers to it
//  Notice that this is the case although we already extracted most of the code that
//  actually performs the actions into classes such as CItemDragOperation,
//  CLassoSelectionOperation and CSelectionAccess (which are not part of the refactoring)
//
//## ISSUE: Mixing concerns
//  We mix lines of code which handle different functionality: A few lines handle
//  lasso selection, then a few lines handle drag-move,...
//  This makes it hard to read and understand a single feature. E.g. if you are looking
//  for a bug in drag-move, you always have to skip the lines not relevant to it
// The "Single responsibility principle" is a good principle
//  to follow in software design: A symbol (a function, a class)
//  should ideally have one job, and one job only.
// This makes for smaller, cleanly defined symbols that are
//  also easier to re-use (reducing code duplication, too)


// Just a wrapper to protect against dereferencing a nullptr
QQuickItem* CWindowEventFilter::getViewRootObject()
{
    if ( ! m_pView)
    {
        return {};
    }
    return m_pView->rootObject();
}

// Just a wrapper to protect against dereferencing a nullptr
QQuickItem* CWindowEventFilter::getLassoItem()
{
    auto* const pViewRootObject = getViewRootObject();
    if ( ! pViewRootObject)
    {
        return{};
    }
    // Simple caching: I only need to find the lasso item the first time I call this function
    if ( ! m_pLasso)
    {
        m_pLasso = CLassoAccess::getLassoItem(pViewRootObject);
    }
    return m_pLasso;
}

// That's the whole thing. The upside of this version is that it has the least code of all versions.
// If three features are all you ever need to implement, maybe this is all-rightish
//  But it will become a nightmare as it grows, so let's try to improve...see 02_FUNCTIONS

