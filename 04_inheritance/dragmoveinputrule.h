#ifndef DRAGMOVEINPUTRULE_H
#define DRAGMOVEINPUTRULE_H

#include "abstractinputrule.h"
#include <QPointer>
#include <QPointF>
#include <itemdragoperation.h>
class QQuickItem;


//## CDragMoveInputRule inherits from CAbstractInputRule, so it uses
//  the enable/disable and rule ID implemented there
// Apart from that (and the name of the event handler functions), not
//  a lot has changed. Let's look at the implementation...
class CDragMoveInputRule : public CAbstractInputRule
{
public:
    static constexpr const char* const sm_pRuleId = "DragMoveRule";
    CDragMoveInputRule();
    ~CDragMoveInputRule() override = default;

    bool handleSinglePointEvent(QSinglePointEvent& event, const CInputInfo& inputInfo) override;
    bool handleKeyEvent(QKeyEvent& event, const CInputInfo& inputInfo) override;

private:
    // Used for one event sequence
    bool m_bDragPossible = false;
    QPointF m_DragStartPos{};
    CItemDragOperation m_DragOperation{};

    static constexpr qreal sm_ItemDragTreshold = 5.0;
};

#endif // DRAGMOVEINPUTRULE_H
