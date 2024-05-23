#ifndef SELECTIONINPUTRULE_H
#define SELECTIONINPUTRULE_H

#include <QPointer>

class QSinglePointEvent;
class QQuickItem;
class QKeyEvent;

class CSelectionInputRule
{
public:
    bool handleSelectionEvent(QSinglePointEvent& event, QQuickItem* pItemUnderCursor, QQuickItem* const pViewRootObject);
    bool handleSelectionEvent(QKeyEvent& event, QQuickItem* const pViewRootObject);
private:
    //## IMPROVEMENT: All members of this class now serve the
    //  job of implementing a click selection feature

    // Members used for one event sequence
    QPointer<QQuickItem> m_pLeftPressedItem{};
};

#endif // SELECTIONINPUTRULE_H
