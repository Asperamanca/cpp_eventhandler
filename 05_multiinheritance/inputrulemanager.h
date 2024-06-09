#ifndef INPUTRULEMANAGER_H
#define INPUTRULEMANAGER_H

#include <vector>
#include <memory>
#include "inputrule.h"

class CEventInfo;

using IOwnedInputRulePtr = std::unique_ptr<CInputRule>;

class CInputRuleManager
{
public:
    void appendInputRule(std::unique_ptr<CInputRule> pInputRule);
    void setRuleDisabled(const QString& ruleId, bool bDisabled);

    //## IMPROVEMENT: We have simplified the interface of CInputRuleManager
    // Let's read on in the cpp file...
    bool passEventThroughRules(const CEventInfo& eventInfo);

private:
    std::vector<IOwnedInputRulePtr> m_InputRules;
};

#endif // INPUTRULEMANAGER_H
