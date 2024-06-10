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

    pInputRuleManager->setRuleDisabled(CLassoInputRule::sm_pRuleId, ! cmdLineParser.isLassoAllowed());
    pInputRuleManager->setRuleDisabled(CDragMoveInputRule::sm_pRuleId, ! cmdLineParser.isEditAllowed());

    return pInputRuleManager;
}
