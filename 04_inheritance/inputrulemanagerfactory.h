#ifndef INPUTRULEMANAGERFACTORY_H
#define INPUTRULEMANAGERFACTORY_H

#include <commandlineparser.h>
#include <memory>
#include "inputrulemanager.h"

class CInputRuleManagerFactory
{
public:
    //## Creates, configures and returns an input rule manager
    // We return a unique pointer of a CInputRuleManager
    //  That means we not only return the created object, but we also
    //  pass ownership of the object to the caller, i.e. the caller is now
    //  responsible for the object's lifetime.
    // std::unique_ptr will help us manage that lifetime: It automatically
    //  deletes the object when it goes out of scope
    // Because it is not copyable, it also prevents issues such as duplicate ownership, double deletes, etc.
    static std::unique_ptr<CInputRuleManager> createInputRuleManager(const CCommandLineParser& cmdLineParser);
};

#endif // INPUTRULEMANAGERFACTORY_H
