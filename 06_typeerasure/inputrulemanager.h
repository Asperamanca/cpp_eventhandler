#ifndef INPUTRULEMANAGER_H
#define INPUTRULEMANAGER_H

#include <vector>
#include "inputrule.h"

class CEventInfo;


class CInputRuleManager
{
public:
    void appendInputRule(const CInputRule& inputRule);
    void setRuleDisabled(const QString& ruleId, bool bDisabled);
    bool passEventThroughRules(const CEventInfo& eventInfo);

private:
    //## IMPROVEMENT: We no longer store pointers, we store values
    //  How is that possible?
    // CInputRule is now a concrete class, no longer just an interface class.
    //  It holds an event handler ("has a"-relationship),
    //  it no longer "is an" event handler.
    // "Has-a"-relationships (also called composition) are in general more flexible
    //  to use than "is-a"-relationships, which are also called inheritance.
    // That said, you still need to use both in your designs, depending on what
    //  you try to achieve. Many powerful design patterns use some combination of both.
    //
    // Let's head over to inputrulemanagerfactory.cpp, and see
    //  how the new CInputRule is created...
    std::vector<CInputRule> m_InputRules;
};

#endif // INPUTRULEMANAGER_H
