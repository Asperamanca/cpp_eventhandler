#include "windoweventfilter.h"
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
    m_DragMoveRule.setEditAllowed(bArg);
}

void CWindowEventFilter::setLassoAllowed(const bool bArg)
{
    m_LassoRule.setLassoAllowed(bArg);
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
        // Here is one imporant change: I now also pass pViewRootObject since I cannot simply get
        //  it inside the classes
        if (m_LassoRule.handleLassoEvent(*pPointerEvent, pItemUnderCursor, pViewRootObject))
        {
            return false;
        }
        if (m_DragMoveRule.handleDragMoveEvent(*pPointerEvent, pItemUnderCursor, pViewRootObject))
        {
            return false;
        }
        if (m_SelectionRule.handleSelectionEvent(*pPointerEvent, pItemUnderCursor, pViewRootObject))
        {
            return false;
        }
        //## ISSUE: We now pass 3 parameters to the event handler functions, and two of them
        //  have the same type. This is dangerous, becase we could mix them up easily.
    }
    else if (auto* const pKeyEvent = dynamic_cast<QKeyEvent*>(pEvent))
    {
        if (m_LassoRule.handleLassoEvent(*pKeyEvent,pViewRootObject))
        {
            return true;
        }
        if (m_SelectionRule.handleSelectionEvent(*pKeyEvent, pViewRootObject))
        {
            return true;
        }
    }
    //## ISSUE: While we certainly improved things, does the CWindowEventFilter
    //   really need to know all the details of what features to call in which order...?
    //
    // We also directly include the respective header files
    //  - dragmoveinputrule.h
    //  - lassoinputrule.h
    //  - selectioninputrule.h
    // The term here is "tight coupling": CWindowEventFilter directly knows about each
    //   of the three features we currently have.
    //   Unless we change something, it will also depend on any new feature we may add.
    // Also, the code won't become nicer by adding more and more currently have."else if" blocks...

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

//## Most of the actual code is now moved to
//  - dragmoveinputrule.h/cpp
//  - lassoinputrule.h/cpp
//  - selectioninputrule.h/cpp
// The actual logic is still the same as what we had in the
//  functions of the previous verion (02).
// Take a look at these files for a few more comments, before
//  moving on to the next refinement...

