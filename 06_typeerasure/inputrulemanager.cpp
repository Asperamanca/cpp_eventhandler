#include "inputrulemanager.h"
#include "eventinfo.h"
#include "inputinfo.h"
#include <ranges>

void CInputRuleManager::appendInputRule(const CInputRule& inputRule)
{
    m_InputRules.emplace_back(std::move(inputRule));
}

void CInputRuleManager::setRuleDisabled(const QString& ruleId, bool bDisabled)
{
    //## IMPROVEMENT: Since we hold values, not pointers,
    //  we don't have to check for nullptr anymore
    auto iter = std::ranges::find_if(m_InputRules,[&ruleId](const CInputRule& rule)
                                    {
                                        return (rule.getRuleId() == ruleId);
                                    });
    if (iter != m_InputRules.end())
    {
        iter->setDisabled(bDisabled);
    }
}


namespace
{

auto getEnabledRules(std::vector<CInputRule>& inputRules)
{
    //## IMPROVEMENT: Since we hold values, not pointers,
    //  we don't have to check for nullptr anymore
    return inputRules |
           std::views::filter([](const CInputRule& inputRule){return ! inputRule.isDisabled();});
}

} // namespace


bool CInputRuleManager::passEventThroughRules(const CEventInfo& eventInfo)
{
    auto enabledRules = getEnabledRules(m_InputRules);
    CInputInfo inputInfo(eventInfo);
    //## IMPROVEMENT: Single loop, simple code
    // The dynamic_casts for event types have moved into CInputInfo, so
    //  maybe take a quick look there
    for(auto& rule : enabledRules)
    {
        if (rule.handleEvent(inputInfo))
        {
            return true;
        }
    }
    return false;
}


