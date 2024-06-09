#ifndef SELECTIONINPUTRULE_H
#define SELECTIONINPUTRULE_H

#include "inputrule.h"
#include "eventhandler.h"
#include <QPointer>
class QQuickItem;

class CSelectionInputRule : public CInputRule,
                            public ISinglePointEventHandler,
                            public IKeyEventHandler
{
public:
    static constexpr const char* const sm_pRuleId = "SelectionRule";
    CSelectionInputRule();
    ~CSelectionInputRule() override = default;

    bool handleSinglePointEvent(const QSinglePointEvent& event, const CInputInfo& inputInfo) override;
    bool handleKeyEvent(const QKeyEvent& event, const CInputInfo& inputInfo) override;

private:
    // Used for one event sequence
    QPointer<QQuickItem> m_pLeftPressedItem{};
};

#endif // SELECTIONINPUTRULE_H
