#ifndef DRAGMOVEEVENTHANDLER_H
#define DRAGMOVEEVENTHANDLER_H

#include "eventhandler.h"
#include <QPointer>
#include <QPointF>
#include <itemdragoperation.h>
class QQuickItem;


//## CDragMoveEventHandler now only handles events, nothing more
// It only needs single point events, so it only inherits from that event handler.
class CDragMoveEventHandler : public ISinglePointEventHandler
{
public:
    ~CDragMoveEventHandler() override = default;
    static constexpr const char* const sm_pRuleId = "DragMoveRule";

    bool handleSinglePointEvent(const QSinglePointEvent& event, const CInputInfo& inputInfo) override;
    //## IMPROVEMENT: No dummy function needed for event functions we don't need
private:
    // Used for one event sequence
    bool m_bDragPossible = false;
    QPointF m_DragStartPos{};
    CItemDragOperation m_DragOperation{};

    static constexpr qreal sm_ItemDragTreshold = 5.0;
};


#endif // DRAGMOVEEVENTHANDLER_H
