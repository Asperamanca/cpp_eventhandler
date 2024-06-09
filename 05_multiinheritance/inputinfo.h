#ifndef INPUTINFO_H
#define INPUTINFO_H

#include "eventinfo.h"

//## This class changed a lot. Let's look at the changes piece by piece.
// 1) We needed to add the event itself to CInputInfo, in order to avoid a separate parameter
// 2) We get constructed from a CEventInfo, which already contains the event and the ViewRootObject
//    It makes sense to simply store the whole CEventInfo structure (after all, I need all the data it holds),
//    but I don't want to expose CEventInfo to all the classes using CInputInfo.
//    CEventInfo is for the CREATOR of CInputInfo, not for the USER.
//    Why is this distinction important?
//    As it is now, CEventInfo is a means to communicate between CWindowEventFilter, CInputManager and CInputInfo
//    It would be relatively easy to change it without affecting too much code
//    If I force every event handler to use CEventInfo, it will eventually become hard to change it, because many
//    classes use it. By providing a separate interface for the users for CInputInfo, I retain some flexibility, and
//    can change the interfaces for CEventInfo and CInputInfo independently.
// 3) In order to access data from CEventInfo (and because I don't want to expose it directly), I use getter functions
//    and make the data private. I could still make m_pItemUnderCursor a public data member if I wanted to, but I prefer
//    consistent interfaces, therefore all data members are now private, and all access is via getter functions.
//
// Let's see the cpp file for a few more comments, then head back to inputrulemanager.cpp...
class CInputInfo
{
public:
    explicit CInputInfo(const CEventInfo& eventInfo);

    const QEvent* getEvent() const;
    QPointer<QQuickItem> getViewRootObject() const;
    QPointer<QQuickItem> getItemUnderCursor() const;

private:
    static QPointer<QQuickItem> determineItemUnderCursor(const CEventInfo& eventInfo);

private:
    CEventInfo m_EventInfo;
    QPointer<QQuickItem> m_pItemUnderCursor{};
};

#endif // INPUTINFO_H
