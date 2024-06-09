#ifndef INPUTRULEMANAGERFACTORY_H
#define INPUTRULEMANAGERFACTORY_H

#include <commandlineparser.h>
#include <memory>
#include "inputrulemanager.h"

class CInputRuleManagerFactory
{
public:
    static std::unique_ptr<CInputRuleManager> createInputRuleManager(const CCommandLineParser& cmdLineParser);
};

#endif // INPUTRULEMANAGERFACTORY_H
