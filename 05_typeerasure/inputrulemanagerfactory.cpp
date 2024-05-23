#include "inputrulemanagerfactory.h"
#include "inputrulemanager.h"
#include "lassoeventhandler.h"
#include "dragmoveeventhandler.h"
#include "selectioneventhandler.h"

std::unique_ptr<CInputRuleManager> CInputRuleManagerFactory::createInputRuleManager(const CCommandLineParser& cmdLineParser)
{
    auto pInputRuleManager = std::make_unique<CInputRuleManager>();

    //## The major change is visible here: We simply create a value of type CInputRule,
    //  and pass a value of an event handler type.
    // But how does that work? After all, CLassoEventHandler, CDragMoveEventHandler and
    //  CSelectionEventHandler are all different types...?
    //
    // Now we come to the centerpiece of this refinement.
    //  Let's head over to inputrule.h/cpp
    pInputRuleManager->appendInputRule(CInputRule(CLassoEventHandler(),CLassoEventHandler::sm_pRuleId));
    pInputRuleManager->appendInputRule(CInputRule(CDragMoveEventHandler(),CDragMoveEventHandler::sm_pRuleId));
    pInputRuleManager->appendInputRule(CInputRule(CSelectionEventHandler(),CSelectionEventHandler::sm_pRuleId));

    pInputRuleManager->setRuleDisabled(CLassoEventHandler::sm_pRuleId, ! cmdLineParser.isLassoAllowed());
    pInputRuleManager->setRuleDisabled(CDragMoveEventHandler::sm_pRuleId, ! cmdLineParser.isEditAllowed());

    return pInputRuleManager;
}
