#ifndef LASSOINPUTRULE_H
#define LASSOINPUTRULE_H

#include "inputrule.h"
#include "eventhandler.h"
#include <QPointer>
#include <lassoselectionoperation.h>
class QQuickItem;

//## We now inherit from CInputRule to qualify as a valid input rule
//  (and for the bookkeeping parts),
//  and also from ISinglePointEventHandler (to receive single pointer events,
//  such as mouse events) as well as from IKeyEventHandler (for key events)
class CLassoInputRule : public CInputRule,
                        public ISinglePointEventHandler,
                        public IKeyEventHandler
{
public:
    static constexpr const char* const sm_pRuleId = "LassoRule";
    CLassoInputRule();
    ~CLassoInputRule() override = default;

    bool handleSinglePointEvent(const QSinglePointEvent& event, const CInputInfo& inputInfo) override;
    bool handleKeyEvent(const QKeyEvent& event, const CInputInfo& inputInfo) override;

private:
    QQuickItem* getLassoItem(QQuickItem* const pViewRootObject);

private:
    QPointer<QQuickItem> m_pLasso{};

    // Used for one event sequence
    CLassoSelectionOperation m_LassoSelectionOperation{};
};

#endif // LASSOINPUTRULE_H
