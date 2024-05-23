#ifndef INPUTRULE_H
#define INPUTRULE_H

#include <QString>

class QSinglePointEvent;
class QKeyEvent;
class CInputInfo;

//## This is an interface class (also called protocol class)
//  It describes an interface, but no implementations at all
// Interface classes are very useful as most basic base classes because
//  1) They help you to separate concerns ("what is the interface" vs.
//      "what is the implementation"). This also typically makes them easy to read.
//  2) They typically draw few dependencies (because they don't need any implementation,
//      just the types of the interfaces). This makes them easier to use in many places,
//      since they will not "glue your project together with fat dependencies"
//  3) They make writing unit test easier, as you can typically write a mockup implementation
//      that helps your test cases
//  4) It's less scary to use multiple inheritance with interface classes, since you
//      don't need to worry about conflicting implementations ("just" conflicting interface names)
// NAMING: I use the 'I' prefix for such classes
class IInputRule
{
public:
    // Constructor and destructor are defaulted
    IInputRule() = default;
    // Destructor must be virtual, so derived classes will be destroyed completely when deleted
    //  from the base class pointer
    virtual ~IInputRule() = default;

    // Interface for handling events
    virtual bool handleSinglePointEvent(QSinglePointEvent& event, const CInputInfo& inputInfo) = 0;
    virtual bool handleKeyEvent(QKeyEvent& event, const CInputInfo& inputInfo) = 0;

    // Interface for enabling and disabling input rules
    virtual bool isDisabled() const = 0;
    virtual void setDisabled(const bool bArg) = 0;
    // Interface for getting the rule ID
    virtual QString getRuleId() const = 0;
};

#endif // INPUTRULE_H
