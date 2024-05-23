#include "abstractinputrule.h"


CAbstractInputRule::CAbstractInputRule(const QString& ruleId)
    : m_RuleId(ruleId)
{
}

bool CAbstractInputRule::isDisabled() const
{
    return m_bDisabled;
}

void CAbstractInputRule::setDisabled(const bool bArg)
{
    m_bDisabled = bArg;
}

QString CAbstractInputRule::getRuleId() const
{
    return m_RuleId;
}
