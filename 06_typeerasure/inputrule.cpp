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

CInputRule& CInputRule::operator=(CInputRule other)
{
    // This is an implementation of the copy-and-swap idiom.
    //  The copy already happened when I passed the parameter "other" by value
    //  Now I do a swap...and the copy of "other" becomes...me
    // As opposed to other ways to implement the copy constructor, it is safe
    //  not to check for self-assignment, because I always work with two distinct
    //  object instances - in the case of self-assignment with 'this' and a copy of 'this'
    swap(*this, other);
    return *this;
}

void swap(CInputRule& left, CInputRule& right) noexcept
{
    std::swap(left.m_pPrivate, right.m_pPrivate);
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

    if ( ! inputInfo.getEvent())
    {
        return false;
    }

    if (auto* const pPointerEvent = inputInfo.getSinglePointEvent())
    {
        return pEventHandlerWrapper->handleSinglePointEvent(*pPointerEvent,inputInfo);
    }
    else if (auto* const pKeyEvent = inputInfo.getKeyEvent())
    {
        return pEventHandlerWrapper->handleKeyEvent(*pKeyEvent, inputInfo);
    }
    return false;
}
