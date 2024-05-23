#include "windoweventfilter.h"
#include "inputrulemanager.h"
#include "inputinfo.h"
#include <QEvent>
#include <QQuickView>
#include <QQuickItem>
#include <QSinglePointEvent>

CWindowEventFilter::CWindowEventFilter(std::unique_ptr<CInputRuleManager> pInputRuleManager, QObject* pParent)
    : QObject{pParent},
      m_pInputRuleManager(std::move(pInputRuleManager))
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
bool CWindowEventFilter::eventFilter([[maybe_unused]] QObject* pWatched, QEvent* pEvent)
{
    auto* const pViewRootObject = getViewRootObject();
    if (( ! pEvent) || ( ! pViewRootObject) || ( ! m_pInputRuleManager) )
    {
        return false;
    }

    if (auto* const pPointerEvent = dynamic_cast<QSinglePointEvent*>(pEvent))
    {
        auto* const pItemUnderCursor = pViewRootObject->childAt(pPointerEvent->scenePosition().x(),
                                                                pPointerEvent->scenePosition().y());
        // Now is a good time to take a quick look at inputinfo.h/cpp before continuing here...
        //## IMPROVEMENT: No more confusing the parameters because we have a single input info
        // What you see below is aggregate initialization with designated initializers
        //  Because I use names to assign values to the class members, it becomes much harder
        //  for me to mix up parameters
        CInputInfo inputInfo{.m_pItemUnderCursor = pItemUnderCursor,
                             .m_pViewRootObject = pViewRootObject};
        //## IMPROVEMENT: We replaced the ugly "else if" ladder with a single call
        // Let's head into inputrulemanager.h/cpp and see where the events are headed...
        m_pInputRuleManager->passSinglePointerEventThroughRules(*pPointerEvent, inputInfo);
    }
    else if (auto* const pKeyEvent = dynamic_cast<QKeyEvent*>(pEvent))
    {
        CInputInfo inputInfo{.m_pItemUnderCursor = nullptr,
                             .m_pViewRootObject = pViewRootObject};
        m_pInputRuleManager->passKeyEventThroughRules(*pKeyEvent, inputInfo);
    }
    //## ISSUE: Can't we just pass any kind of event to the input rule manager?
    // Do we really need this else if ladder of code here?

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


