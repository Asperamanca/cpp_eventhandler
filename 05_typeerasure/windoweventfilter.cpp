#include "windoweventfilter.h"
#include "inputrulemanager.h"
#include "eventinfo.h"
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

    //## IMPROVEMENT: We no longer need to distinguish by event type here
    // In order to achieve that, we simply pass all events on to the input rule manager,
    //  and also use a custom type to pass all needed parameters in one piece
    //  (a trick we have used before - you can take a peek at eventinfo.h)
    // Let's head into inputrulemanager.h/cpp and follow the trail...
    CEventInfo eventInfo{.m_pEvent = pEvent,
                         .m_pViewRootObject = pViewRootObject};
    m_pInputRuleManager->passEventThroughRules(eventInfo);

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


