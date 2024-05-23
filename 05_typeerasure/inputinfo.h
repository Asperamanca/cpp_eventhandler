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

private:
    void updateItemUnderCursor() const;

private:
    CEventInfo m_EventInfo;
    mutable std::optional<QPointer<QQuickItem>> m_opCachedItemUnderCursor{};
};

#endif // INPUTINFO_H
