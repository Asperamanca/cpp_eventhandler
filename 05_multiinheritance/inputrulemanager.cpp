#include "inputrulemanager.h"
#include "eventinfo.h"
#include "inputinfo.h"
#include "eventhandler.h"
#include <ranges>


void CInputRuleManager::appendInputRule(std::unique_ptr<CInputRule> pInputRule)
{
    m_InputRules.emplace_back(std::move(pInputRule));
}

void CInputRuleManager::setRuleDisabled(const QString& ruleId, bool bDisabled)
{
    auto iter = std::ranges::find_if(m_InputRules,[&ruleId](const std::unique_ptr<CInputRule>& pRule)
                                    {
                                        if ( ! pRule) { return false;}          // nullptr check
                                        return (pRule->getRuleId() == ruleId);  // check for matching id
                                    });
    if (iter != m_InputRules.end())
    {
        (*iter)->setDisabled(bDisabled);
    }
}

namespace
{

auto getEnabledRules(const std::vector<IOwnedInputRulePtr>& inputRules)
{
    return inputRules |
           std::views::filter([](const IOwnedInputRulePtr& pInputRule){return pInputRule.get();}) |
           std::views::filter([](const IOwnedInputRulePtr& pInputRule){return ! pInputRule->isDisabled();});
}

} // namespace

bool CInputRuleManager::passEventThroughRules(const CEventInfo& eventInfo)
{
    auto enabledRules = getEnabledRules(m_InputRules);
    //## Creating the inputInfo is my responsibility now
    //  CInputInfo now has a constructor that creates it directly from the eventInfo
    //  Let's go to inputinfo.h to see more changes...
    CInputInfo inputInfo(eventInfo);

    // First see if the event is a single point event...
    if (const auto* const pSinglePointerEvent = dynamic_cast<const QSinglePointEvent*>(eventInfo.m_pEvent))
    {
        for(const auto& pRule : enabledRules)
        {
            // Pass it to any rule which implements ISinglePointEventHandler
            // How does that work now? Let's look at dragmoveinputrule.h...
            if (auto* const pSinglePointEventHandler = dynamic_cast<ISinglePointEventHandler*>(pRule.get()))
            {
                if (pSinglePointEventHandler->handleSinglePointEvent(*pSinglePointerEvent, inputInfo))
                {
                    // If the rule decides to block the event, we stop further processing
                    return true;
                }
            }
        }
    }
    // If it's not a single point event, maybe it's a key event...
    else if (const auto* const pKeyEvent = dynamic_cast<const QKeyEvent*>(eventInfo.m_pEvent))
    {
        for(const auto& pRule : enabledRules)
        {
            // Pass it to any rule which implements IKeyEventHandler
            if (auto* const pKeyEventHandler = dynamic_cast<IKeyEventHandler*>(pRule.get()))
            {
                if (pKeyEventHandler->handleKeyEvent(*pKeyEvent, inputInfo))
                {
                    return true;
                }
            }
        }
    }
    //## ISSUE: The loop is repeated for each type of event. Can we somehow avoid this duplication?
    //## ISSUE: We need to dynamic_cast our event handlers a lot. Is there some other way to do it?

    return false;
}
