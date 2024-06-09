#ifndef INPUTRULE_H
#define INPUTRULE_H

#include <QString>
#include <memory>
#include "inputruleprivate.h"

class QEvent;
class CInputInfo;

//## CInputRule is an entirely new class
// If you look at the second half of the class declaration, you'll notice that it offers
//  the "bookkeeping" part of the previous input rule (enable/disable and rule ID)
// But before that comes the interesting part...
class CInputRule
{
public:
    //## Since we pass completely unrelated types to the constructor,
    // we implement a constructor accepting a template argument
    //  The template argument can be any type, really
    // Ideally, it inherits from one of the event handler interface classes
    //  declared in eventhandler.h, but currently, even passing an int would compile.
    //  (We could add a check that forces the caller to pass something that inherits from
    //  at least one event handler. It would also make a nice exercise once you are done reading)
    template<class TEventHandler>
    explicit CInputRule(TEventHandler eventHandler, QString ruleId);

    // To make it a fully working value type, we implement default constructor,
    //  copy and move constructors and their corresponding assignment operators,
    //  and a destructor (for the sake of completeness - see also: https://en.cppreference.com/w/cpp/language/rule_of_three)
    CInputRule();
    CInputRule(const CInputRule& other);
    CInputRule& operator=(const CInputRule& other);
    // In case you haven't seen one of those before: This is a move constructor
    //  and the '&&' is a rvalue reference declarator
    // If you want to know more about that, look up "C++ value categories"
    CInputRule(CInputRule&& other) = default;
    // This is the move assignment operator, the companion to the move constructor
    CInputRule& operator=(CInputRule&& other) = default;
    ~CInputRule() = default;

    bool isDisabled() const;
    void setDisabled(const bool bArg);

    QString getRuleId() const;

    bool handleEvent(const CInputInfo& inputInfo);
private:
    std::unique_ptr<CInputRulePrivate> m_pPrivate;
};

//## The implementation of the templated constructor creates a unique_ptr to a private class,
//  just passing on the arguments by moving them.
//  Let's take a look at inputruleprivate.h next...
template<class TEventHandler>
CInputRule::CInputRule(TEventHandler eventHandler, QString ruleId)
    : m_pPrivate(std::make_unique<CInputRulePrivate>(std::move(eventHandler), std::move(ruleId)))
{
}

#endif // INPUTRULE_H
