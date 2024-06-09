#ifndef LASSOINPUTRULE_H
#define LASSOINPUTRULE_H

#include "inputrule.h"
#include <QPointer>
#include <lassoselectionoperation.h>
class QQuickItem;

class CLassoInputRule : public CInputRule
{
public:
    static constexpr const char* const sm_pRuleId = "LassoRule";
    CLassoInputRule();
    ~CLassoInputRule() override = default;

    bool handleSinglePointEvent(QSinglePointEvent& event, const CInputInfo& inputInfo) override;
    bool handleKeyEvent(QKeyEvent& event, const CInputInfo& inputInfo) override;

private:
    QQuickItem* getLassoItem(QQuickItem* const pViewRootObject);

private:
    QPointer<QQuickItem> m_pLasso{};

    // Used for one event sequence
    CLassoSelectionOperation m_LassoSelectionOperation{};
};

#endif // LASSOINPUTRULE_H
