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

    // Enabling and disabling features is now in the domain of the input rule manager
    //  However, we cannot expect a function like "setEditAllowed", since
    //  the input rule manager does not even know what "Edit" means.
    // Instead, we use a generic function which uses a unique name ("RuleId")
    //  to enable or disable single features
    pInputRuleManager->setRuleDisabled(CLassoInputRule::sm_pRuleId, ! cmdLineParser.isLassoAllowed());
    pInputRuleManager->setRuleDisabled(CDragMoveInputRule::sm_pRuleId, ! cmdLineParser.isEditAllowed());

    return pInputRuleManager;
}
