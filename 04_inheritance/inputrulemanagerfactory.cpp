#include "inputrulemanagerfactory.h"
#include "lassoinputrule.h"
#include "dragmoveinputrule.h"
#include "selectioninputrule.h"

std::unique_ptr<CInputRuleManager> CInputRuleManagerFactory::createInputRuleManager(const CCommandLineParser& cmdLineParser)
{
    auto pInputRuleManager = std::make_unique<CInputRuleManager>();

    pInputRuleManager->appendInputRule(std::make_unique<CLassoInputRule>());
    pInputRuleManager->appendInputRule(std::make_unique<CDragMoveInputRule>());
    pInputRuleManager->appendInputRule(std::make_unique<CSelectionInputRule>());

    //## IMPROVEMENT: Creating the input rules is now in a separate class and file.
    //  The way how to add an input rule is now generic ("appendInputRule")
    //  If we want to customize which input rules to create (e.g. for test cases),
    //  we now have a nice place to do that.
    // Options for customization include:
    //  1) Add rules conditionally based on command line
    //  2) Use #ifdef to create or not create certain rules based on compiler flags
    //  3) Link a different inputrulemanagerfactory.cpp for other use cases
    // (We could have done option 2 in the previous version also, but that would have
    //  affected our central class CWindowEventHandler)

    // Enabling and disabling features is now in the domain of the input rule manager
    //  However, we cannot expect a function like "setEditAllowed", since
    //  the input rule manager does not even know what "Edit" means.
    // Instead, we use a generic function which uses a unique name ("RuleId")
    //  to enable or disable single features
    pInputRuleManager->setRuleDisabled(CLassoInputRule::sm_pRuleId, ! cmdLineParser.isLassoAllowed());
    pInputRuleManager->setRuleDisabled(CDragMoveInputRule::sm_pRuleId, ! cmdLineParser.isEditAllowed());

    return pInputRuleManager;
}
