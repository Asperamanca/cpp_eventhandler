#ifndef ABSTRACTINPUTRULE_H
#define ABSTRACTINPUTRULE_H

#include "inputrule.h"
#include <QString>

//## CAbstractInputRule implements the common part that works the same for all input rules
// I call it 'Abstract', because it does not implement all virtual functions,
//  therefore you can never create a "CAbstractInputRule" on it's own. You have
//  to derive from it and make it complete.
class CAbstractInputRule : public IInputRule
{
public:
    CAbstractInputRule() = delete;
    explicit CAbstractInputRule(const QString& ruleId);

    bool isDisabled() const override;
    void setDisabled(const bool bArg) override;
    QString getRuleId() const override;

private:
    bool m_bDisabled{false};
    const QString m_RuleId;
};

//## ISSUE: This class basically exists to avoid duplicating code between rules
//  We don't want to implement isDisabled() / setDisabled() in every rule, because
//  the code is always the same, anyway.
//
//  While it's relatively common in object oriented programming to use a base class
//  to prevent code duplication, the approach is far from ideal:
//
//  One disadvantage is that you lose flexibility, because you can either use the whole
//  class, or nothing at all. If you have a large base class, and you want to share
//  some code most of the time, but not all the time, then you start to write default
//  implementations in the base class, which you sometimes override in the derived classes...
//  If this gets big, good luck finding out what gets called when!
//
//  Also, as things grow, such base classes are natural places to accumulate tons of code,
//  way more than would be healthy for a single file.

#endif // ABSTRACTINPUTRULE_H
