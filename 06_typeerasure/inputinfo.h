#ifndef INPUTINFO_H
#define INPUTINFO_H

#include "eventinfo.h"
#include <optional>

class CInputInfo
{
public:
    explicit CInputInfo(const CEventInfo& eventInfo);

    const QEvent* getEvent() const;
    QPointer<QQuickItem> getViewRootObject() const;
    QPointer<QQuickItem> getItemUnderCursor() const;
    const QSinglePointEvent* getSinglePointEvent() const;
    const QKeyEvent* getKeyEvent() const;

private:
    static QPointer<QQuickItem> determineItemUnderCursor(const CEventInfo& eventInfo);

private:
    CEventInfo m_EventInfo;
    mutable std::optional<QPointer<QQuickItem>> m_opCachedItemUnderCursor{};
    const QSinglePointEvent* const m_pSinglePointEvent{};
    const QKeyEvent* const m_pKeyEvent{};
};

#endif // INPUTINFO_H
