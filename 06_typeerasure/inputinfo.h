#ifndef INPUTINFO_H
#define INPUTINFO_H

#include "eventinfo.h"

class CInputInfo
{
public:
    explicit CInputInfo(const CEventInfo& eventInfo);

    const QEvent* getEvent() const;
    QPointer<QQuickItem> getViewRootObject() const;
    QPointer<QQuickItem> getItemUnderCursor() const;
    // Multiple input rules will now be called with the same CInputInfo, without
    //  passing the typed event directly anymore.
    // In order to avoid unnecessary dynamic_casts, I cache the typed pointers here
    const QSinglePointEvent* getSinglePointEvent() const;
    const QKeyEvent* getKeyEvent() const;

private:
    static QPointer<QQuickItem> determineItemUnderCursor(const CEventInfo& eventInfo);

private:
    CEventInfo m_EventInfo;
    QPointer<QQuickItem> m_pItemUnderCursor{};
    const QSinglePointEvent* const m_pSinglePointEvent{};
    const QKeyEvent* const m_pKeyEvent{};
};

#endif // INPUTINFO_H
