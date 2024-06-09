#ifndef EVENTINFO_H
#define EVENTINFO_H

#include <QPointer>
#include <QQuickItem>
#include <QEvent>

class CEventInfo
{
public:
    QEvent* m_pEvent{};
    QPointer<QQuickItem> m_pViewRootObject{};
};

#endif // EVENTINFO_H
