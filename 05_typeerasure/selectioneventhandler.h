#ifndef SELECTIONEVENTHANDLER_H
#define SELECTIONEVENTHANDLER_H

#include "eventhandler.h"
#include <QPointer>
class QQuickItem;

class CSelectionEventHandler : public ISinglePointEventHandler,
                               public IKeyEventHandler
{
public:
    ~CSelectionEventHandler() override = default;
    static constexpr const char* const sm_pRuleId = "SelectionRule";

    bool handleSinglePointEvent(const QSinglePointEvent& event, const CInputInfo& inputInfo) override;
    bool handleKeyEvent(const QKeyEvent& event, const CInputInfo& inputInfo) override;

private:
    // Used for one event sequence
    QPointer<QQuickItem> m_pLeftPressedItem{};
};

#endif // SELECTIONEVENTHANDLER_H
