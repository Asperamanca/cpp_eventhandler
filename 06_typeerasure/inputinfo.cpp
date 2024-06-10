#include "inputinfo.h"
#include <QSinglePointEvent>
#include <QKeyEvent>

CInputInfo::CInputInfo(const CEventInfo& eventInfo)
    : m_EventInfo(eventInfo),
      m_pItemUnderCursor{determineItemUnderCursor(eventInfo)},
      m_pSinglePointEvent(dynamic_cast<const QSinglePointEvent*>(eventInfo.m_pEvent)),
      m_pKeyEvent(dynamic_cast<const QKeyEvent*>(eventInfo.m_pEvent))
      // This code may be slightly slower than in our previous refinement: I now ALWAYS do
      //    two dynamic_casts, instead of maybe only one if the first one succeeds
      //    Here, simplicity seems more important until profiling shows that this optimization
      //    is actually relevent.
{
}

const QEvent* CInputInfo::getEvent() const
{
    return m_EventInfo.m_pEvent;
}

QPointer<QQuickItem> CInputInfo::getViewRootObject() const
{
    return m_EventInfo.m_pViewRootObject;
}

QPointer<QQuickItem> CInputInfo::getItemUnderCursor() const
{
    return m_pItemUnderCursor;
}

const QSinglePointEvent* CInputInfo::getSinglePointEvent() const
{
    return m_pSinglePointEvent;
}

const QKeyEvent* CInputInfo::getKeyEvent() const
{
    return m_pKeyEvent;
}

QPointer<QQuickItem> CInputInfo::determineItemUnderCursor(const CEventInfo& eventInfo)
{
    if ( ! eventInfo.m_pViewRootObject)
    {
        return {};
    }
    if (auto* const pPointerEvent = dynamic_cast<const QSinglePointEvent*>(eventInfo.m_pEvent))
    {
        auto* const pItemUnderCursor = eventInfo.m_pViewRootObject->childAt(pPointerEvent->scenePosition().x(),
                                                                            pPointerEvent->scenePosition().y());
        return {pItemUnderCursor};
    }
    return {};
}

