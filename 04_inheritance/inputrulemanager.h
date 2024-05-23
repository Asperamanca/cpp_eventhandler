#ifndef INPUTRULEMANAGER_H
#define INPUTRULEMANAGER_H

#include <vector>
#include <memory>
#include "inputrule.h"

class CInputInfo;
class QKeyEvent;

// IInputRule is a pure interface class (i.e. zero code) that allows
//  us to pass events. Head over to inputrule.h if you'd like to know more

// Sometimes, giving a special type a name improves readability
// That's where the 'using' declaration comes in handy
using IOwnedInputRulePtr = std::unique_ptr<IInputRule>;

//## CInputRuleManager owns and manages a number of input rules
//  Each input rule responds to UI events to implement a specific feature
//  (e.g. lasso selection)
// However, the input manager does not need to know these details: It only knows
//  that it holds a list of input rules, and needs to pass events to them.
// It also offers an interface for enabling/disabling input rules by rule ID
//  (a string that must be unique)
class CInputRuleManager
{
public:
    // When we append a new input rule, we have to pass ownership to the input rule manager
    void appendInputRule(std::unique_ptr<IInputRule> pInputRule);

    // We can enable / disable rules through their rule ID
    void setRuleDisabled(const QString& ruleId, bool bDisabled);

    // These function are called to pass on event. Let's continue in the cpp file...
    bool passSinglePointerEventThroughRules(QSinglePointEvent& event, const CInputInfo& inputInfo);
    bool passKeyEventThroughRules(QKeyEvent& event, const CInputInfo& inputInfo);

private:
    // We simply hold a vector of input rules, and own them.
    //  When the input rule manager is destroyed, so are all contained input rules
    std::vector<IOwnedInputRulePtr> m_InputRules;
};

#endif // INPUTRULEMANAGER_H
