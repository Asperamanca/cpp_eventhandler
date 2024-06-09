#ifndef INPUTRULE_H
#define INPUTRULE_H

#include <QString>

class QSinglePointEvent;
class QKeyEvent;
class CInputInfo;

//## CInputRule is now the base class for all my input rules
//  It implements the bookkeeping part of input rules, and asks deriving
//  classes (=the concrete input rules) to implement the event handling functions.
class CInputRule
{
public:
    // Default constructor deleted: We need the rule ID
    CInputRule() = delete;
    CInputRule(const QString& ruleId);

    // Destructor must be virtual, so derived classes will be destroyed completely when deleted
    //  from the base class pointer
    virtual ~CInputRule() = default;

    // Interface for handling events
    // These must be implemented by deriving classes
    virtual bool handleSinglePointEvent(QSinglePointEvent& event, const CInputInfo& inputInfo) = 0;
    virtual bool handleKeyEvent(QKeyEvent& event, const CInputInfo& inputInfo) = 0;

    bool isDisabled() const;
    void setDisabled(const bool bArg);
    QString getRuleId() const;
private:
    bool m_bDisabled{false};
    const QString m_RuleId;
};

#endif // INPUTRULE_H
