#ifndef INPUTRULE_H
#define INPUTRULE_H

#include <QString>

class QSinglePointEvent;
class QKeyEvent;
class CInputInfo;

//## We dropped the virtual event handler functions here, and only kept the bookkeeping
class CInputRule
{
public:
    CInputRule() = delete;
    CInputRule(const QString& ruleId);
    virtual ~CInputRule() = default;

    bool isDisabled() const;
    void setDisabled(const bool bArg);
    QString getRuleId() const;
private:
    bool m_bDisabled{false};
    const QString m_RuleId;
};

//## ISSUE: This class looks like a simple, concrete class.
//  In fact, the only hint that it is meant to be used in an inheritance
//  hierarchy is the virtual destructor.
// This also means that you can create concrete instances of this
//  class directly. This can lead to a dangerous effect called object slicing
//  Let me explain:
// Let's consider CInputRule and CLassoInputRule
//  CInputRule is the base class. It has two data members, and currently
//  uses 40 bytes of memory when created (might vary by compiler)
//  CLassoInputRule is the derived class. It has some additional data
//  members, and uses a total of 120 bytes of memory when created
//  (that includes the 40 bytes of CInputRule)
// Now let's assume we have the following code:
//  CLassoInputRule lassoRule;
// ...and call a function that only takes a CInputRule:
//  void handleEvent(CInputRule inputRule);
// We have made the seemingly innocent decision to pass the inputRule by value
//  This will create a copy, but it will only copy the first 40 bytes!
//  If we call a virtual function inside 'handleEvent', it will likely
//  access an invalid memory location, which may lead to a crash or
//  a security issue in your application.
//
// A possible fix is to disable copying for the base class (and virtual
//  "clone" method if necessary).
// But let's see whether we can't get rid of this strange base class entirely...

#endif // INPUTRULE_H
