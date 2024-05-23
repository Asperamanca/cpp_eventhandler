#include "inputrulemanager.h"
#include "inputinfo.h"
#include <ranges>


void CInputRuleManager::appendInputRule(std::unique_ptr<IInputRule> pInputRule)
{
    m_InputRules.emplace_back(std::move(pInputRule));
}

void CInputRuleManager::setRuleDisabled(const QString& ruleId, bool bDisabled)
{
    // First we try to find the rule via it's ruleId
    // This is not terribly efficient, if we use this a lot, we could create
    //  a separate map or hash table with faster lookup (I left that out for simplicity)
    auto iter = std::ranges::find_if(m_InputRules,[&ruleId](const std::unique_ptr<IInputRule>& pRule)
                                    {
                                        if ( ! pRule) { return false;}          // nullptr check
                                        return (pRule->getRuleId() == ruleId);  // check for matching id
                                    });
    if (iter != m_InputRules.end())
    {
        // If the rule was found, call 'setDisabled'
        //  But who implements it? Let's look at abstractinputrule.h/cpp to see an example
        (*iter)->setDisabled(bDisabled);
    }
}

namespace
{

// This function will return a view of the currently enabled rules
// There are a couple of things to unpack here:
//
// 1) Why is this function inside an empty namespace?
//  As you may have noticed, this function is not a class member function, but a
//  so-called "free" function. True, it only exists in the .cpp file, so nobody from the
//  outside can call it directly (there is no declaration they can use), but the linker that
//  binds the object code into the final binary can find it.
// Especially in a larger project, it could happen that a function
//  with exactly the same name and parameters exists somewhere else.
// If both functions are in the same namespace, too, it may happen that the linker
//  picks the wrong one, and then funny things will start to happen in your program.
// The empty (or anonymous) namespace is a protection against that: Any functions
//  declared inside such a namespace cannot be used outside of this translation unit
//  (for our purposes here, we can understand "a translation unit" as
//  "the contents of this cpp file", although that's not quite accurate)
//
// 2) What does the 'auto' return type mean? What is a "ranges view"?
// The function uses C++20 ranges, and returns a view. A view does not own data,
//  and also does not copy data. You can think of it like a list of operations that
//  you apply to the original data in order to get the final result.
// If your inputs were butter, eggs, flour, sugar, baking soda and chocolate, you can think of the
//  view as the recipe that will guide you to the finished chocolate cookies.
// It is important to note that writing a recipe does not yet need any of the ingrediants - in
//  terms of C++, building a view does not cost any runtime performance. The code prepared within
//  a view will only execute at it is accessed, and also only as often as needed.
//  For example, you implement a filter that checks if a number is a prime number. Then yoou feed it
//  with a list of incrementing integers (1,2,3,4....see std::iota). If you only need the first
//  5 prime numbers, the filter function will only run as often as necessary to give you five prime numbers.
// So, what type does a view have? That's always specific to the rules that make up the view.
//  Since the type is also very long, it's much easier to simply write 'auto' and let the compiler
//  figure out the rest. Maybe your IDE can show you the true type behind if you hover over
//  the 'auto' return type.
auto getEnabledRules(const std::vector<IOwnedInputRulePtr>& inputRules)
{
    // The recipe for the view is as follows:
    //  a) Start with the input rules
    //  b) Send them through a filter that will only let rules pass if they have a non-null pointer
    //  c) Send them through a filter that will only let rules pass if they are enabled
    // So, not only do we get enabled rules only, but we also can rely that no null pointers remain...useful!
    return inputRules |
           std::views::filter([](const IOwnedInputRulePtr& pInputRule){return pInputRule.get();}) |
           std::views::filter([](const IOwnedInputRulePtr& pInputRule){return ! pInputRule->isDisabled();});
}

}

bool CInputRuleManager::passSinglePointerEventThroughRules(QSinglePointEvent& event, const CInputInfo& inputInfo)
{
    // Of course we only want to pass events to input rules that are not disabled
    // How are we going to get those?
    auto enabledRules = getEnabledRules(m_InputRules);
    for(const auto& pRule : enabledRules)
    {
        //## The rules are called in order of insertion, so rules that were inserted first
        //  get first chance to handle and event
        // We just call the virtual function as declared in the base class,
        //  and the concrete implementation of the rule will be called
        // Let's head to dragmoveinputrule.h/cpp and see how that works
        if (pRule->handleSinglePointEvent(event, inputInfo))
        {
            // If the rule decides to block the event, we stop further processing
            return true;
        }
    }
    // No rules blocked, so we can pass the event on to other parts of the window
    return false;
}

bool CInputRuleManager::passKeyEventThroughRules(QKeyEvent& event, const CInputInfo& inputInfo)
{
    auto enabledRules = getEnabledRules(m_InputRules);
    for(const auto& pRule : enabledRules)
    {
        if (pRule->handleKeyEvent(event, inputInfo))
        {
            return true;
        }
    }
    return false;
}
