#include "inputrule.h"
#include <QSinglePointEvent>
#include <QKeyEvent>
#include "inputinfo.h"

CInputRule::CInputRule()
    : m_pPrivate(std::make_unique<CInputRulePrivate>())
{

}

CInputRule::CInputRule(const CInputRule& other)
    : m_pPrivate(CInputRulePrivate::clone(other.m_pPrivate.get()))
{
}

CInputRule& CInputRule::operator=(const CInputRule& other)
{
    CInputRule copy(other);
    std::swap(m_pPrivate, copy.m_pPrivate);
    return *this;
}

bool CInputRule::isDisabled() const
{
    return m_pPrivate->m_bDisabled;
}

void CInputRule::setDisabled(const bool bArg)
{
    m_pPrivate->m_bDisabled = bArg;
}

QString CInputRule::getRuleId() const
{
    return m_pPrivate->m_RuleId;
}

bool CInputRule::handleEvent(const CInputInfo& inputInfo)
{
    auto* const pEventHandlerWrapper = m_pPrivate->m_pEventHandlerWrapper.get();
    if ( ! pEventHandlerWrapper)
    {
        return false;
    }

    const auto* const pEvent = inputInfo.getEvent();
    if ( ! pEvent)
    {
        return false;
    }

    if (auto* const pPointerEvent = dynamic_cast<const QSinglePointEvent*>(pEvent))
    {
        return pEventHandlerWrapper->handleSinglePointEvent(*pPointerEvent,inputInfo);
    }
    else if (auto* const pKeyEvent = dynamic_cast<const QKeyEvent*>(pEvent))
    {
        return pEventHandlerWrapper->handleKeyEvent(*pKeyEvent, inputInfo);
    }
    return false;
}
