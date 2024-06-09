#ifndef EVENTINFO_H
#define EVENTINFO_H

#include <QPointer>
#include <QQuickItem>
#include <QEvent>

// Super-simple, doesn't event have a cpp file - all we need
class CEventInfo
{
public:
    QEvent* m_pEvent{};
    QPointer<QQuickItem> m_pViewRootObject{};
};

#endif // EVENTINFO_H
