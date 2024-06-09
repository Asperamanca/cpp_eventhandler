#ifndef DRAGMOVEINPUTRULE_H
#define DRAGMOVEINPUTRULE_H

#include "inputrule.h"
#include "eventhandler.h"
#include <QPointer>
#include <QPointF>
#include <itemdragoperation.h>
class QQuickItem;

//## CDragMoveInputRule now inherits both from CInputRule and ISinglePointEventHandler
//  CInputRule is responsible for the bookkeeping part, and ISinglePointEventHandler is
//  responsible for handling of single point events (like mouse events)
// Let's take a look at inputrule.h and eventhandler.h...
class CDragMoveInputRule : public CInputRule,
                           public ISinglePointEventHandler
{
public:
    static constexpr const char* const sm_pRuleId = "DragMoveRule";
    CDragMoveInputRule();
    ~CDragMoveInputRule() override = default;

    bool handleSinglePointEvent(const QSinglePointEvent& event, const CInputInfo& inputInfo) override;
    //## IMPROVEMENT: We no longer have to implement a dummy key event handler
private:
    // Used for one event sequence
    bool m_bDragPossible = false;
    QPointF m_DragStartPos{};
    CItemDragOperation m_DragOperation{};

    static constexpr qreal sm_ItemDragTreshold = 5.0;
};

#endif // DRAGMOVEINPUTRULE_H
