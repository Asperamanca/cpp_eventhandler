#ifndef SELECTIONINPUTRULE_H
#define SELECTIONINPUTRULE_H

#include "inputrule.h"
#include <QPointer>
class QQuickItem;

class CSelectionInputRule : public CInputRule
{
public:
    static constexpr const char* const sm_pRuleId = "SelectionRule";
    CSelectionInputRule();
    ~CSelectionInputRule() override = default;

    bool handleSinglePointEvent(QSinglePointEvent& event, const CInputInfo& inputInfo) override;
    bool handleKeyEvent(QKeyEvent& event, const CInputInfo& inputInfo) override;

private:
    // Used for one event sequence
    QPointer<QQuickItem> m_pLeftPressedItem{};
};

#endif // SELECTIONINPUTRULE_H
