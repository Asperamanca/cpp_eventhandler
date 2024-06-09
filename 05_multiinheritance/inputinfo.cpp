#include "inputinfo.h"
#include <QSinglePointEvent>
#include <QKeyEvent>

// In the constructor, I directly initialize m_pItemUnderCursor using a static function
CInputInfo::CInputInfo(const CEventInfo& eventInfo)
    : m_EventInfo(eventInfo),
      m_pItemUnderCursor{determineItemUnderCursor(eventInfo)}
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

// The code in this function used to be in windoweventfilter.cpp
//  I re-structured it, but the logic is still the same
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

