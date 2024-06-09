#ifndef LASSOEVENTHANDLER_H
#define LASSOEVENTHANDLER_H

#include "eventhandler.h"
#include <QPointer>
#include <lassoselectionoperation.h>
class QQuickItem;

//## CLassoEventHandler needs both single point events and key events.
// How to do that? We simply inherit from more than one event handler interface!
// While multiple inheritance can cause a lot of complexity when used with
//  multiple levels of inheritance, and with classes that also contain implementation,
//  it is actually quite safe when done directly with simple interface classes
//  that have no base class themselves.
// You only need to make sure no naming conflicts exist
//
// The next question is: How are we going to use these new event handler classes?
//  Let's head over to windoweventfilter.cpp, and follow the event trail...
class CLassoEventHandler : public ISinglePointEventHandler,
                           public IKeyEventHandler
{
public:
    ~CLassoEventHandler() override = default;
    static constexpr const char* const sm_pRuleId = "LassoRule";

    bool handleSinglePointEvent(const QSinglePointEvent& event, const CInputInfo& inputInfo) override;
    bool handleKeyEvent(const QKeyEvent& event, const CInputInfo& inputInfo) override;

private:
    QQuickItem* getLassoItem(QQuickItem* const pViewRootObject);

private:
    QPointer<QQuickItem> m_pLasso{};

    // Used for one event sequence
    CLassoSelectionOperation m_LassoSelectionOperation{};
};

#endif // LASSOEVENTHANDLER_H
