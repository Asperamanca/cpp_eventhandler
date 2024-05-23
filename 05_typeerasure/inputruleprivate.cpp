#include "inputruleprivate.h"


std::unique_ptr<CInputRulePrivate> CInputRulePrivate::clone(CInputRulePrivate* const pSource)
{
    if ( ! pSource)
    {
        return {};
    }
    // Again, we have just wrapped the copy constructor and return a newly created unique_ptr
    return std::make_unique<CInputRulePrivate>(*pSource);
}

CInputRulePrivate::CInputRulePrivate(const CInputRulePrivate& other)
    : m_pEventHandlerWrapper(other.m_pEventHandlerWrapper ? other.m_pEventHandlerWrapper->clone() : nullptr),
      m_bDisabled(other.m_bDisabled),
      m_RuleId(other.m_RuleId)
{
}
