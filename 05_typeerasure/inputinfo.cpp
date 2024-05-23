#include "inputinfo.h"

CInputInfo::CInputInfo(const CEventInfo& eventInfo)
    : m_EventInfo(eventInfo)
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
    updateItemUnderCursor();
    return m_opCachedItemUnderCursor.value_or(QPointer<QQuickItem>());
}

void CInputInfo::updateItemUnderCursor() const
{
    if (m_opCachedItemUnderCursor.has_value())
    {
        return;
    }

    if ( auto pViewRootObject = getViewRootObject())
    {
        if (auto* const pPointerEvent = dynamic_cast<const QSinglePointEvent*>(getEvent()))
        {
            auto* const pItemUnderCursor = pViewRootObject->childAt(pPointerEvent->scenePosition().x(),
                                                                    pPointerEvent->scenePosition().y());
            m_opCachedItemUnderCursor = QPointer<QQuickItem>(pItemUnderCursor);
            return;
        }
    }
    m_opCachedItemUnderCursor = QPointer<QQuickItem>{};
}
