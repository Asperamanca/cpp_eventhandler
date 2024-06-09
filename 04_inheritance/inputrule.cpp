#include "inputrule.h"


CInputRule::CInputRule(const QString& ruleId)
    : m_RuleId(ruleId)
{
}

bool CInputRule::isDisabled() const
{
    return m_bDisabled;
}

void CInputRule::setDisabled(const bool bArg)
{
    m_bDisabled = bArg;
}

QString CInputRule::getRuleId() const
{
    return m_RuleId;
}

