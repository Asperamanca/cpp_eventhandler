#ifndef DRAGMOVEINPUTRULE_H
#define DRAGMOVEINPUTRULE_H

#include <QPointer>
#include <QPointF>
#include <itemdragoperation.h>

class QSinglePointEvent;
class QQuickItem;


class CDragMoveInputRule
{
public:
    void setEditAllowed(const bool bArg);
    bool handleDragMoveEvent(QSinglePointEvent& event, QQuickItem* pItemUnderCursor, QQuickItem* const pViewRootObject);

private:
    //## IMPROVEMENT: All members of this class now serve the
    //  job of implementing a drag-move feature
    bool m_bEditAllowed = false;

    // Members used for one event sequence
    bool m_bDragPossible = false;
    QPointF m_DragStartPos{};
    CItemDragOperation m_DragOperation{};

    static constexpr qreal sm_ItemDragTreshold = 5.0;
};

#endif // DRAGMOVEINPUTRULE_H
