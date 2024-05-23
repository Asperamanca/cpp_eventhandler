#ifndef INPUTRULEPRIVATE_H
#define INPUTRULEPRIVATE_H

#include <QString>
#include "eventhandler.h"
// #include <QSinglePointEvent>
// #include <QKeyEvent>
// #include "inputinfo.h"
class QSinglePointEvent;
class QKeyEvent;

class CInputInfo;

//## Before we come to CInputHandlerPrivate, we need to look at some helper classes
//  This code follows the pattern of an "owning type erasure" as described in
//  Klaus Iglberger's book "C++ Software Design"
//
// First we define an interface class with some familiar fuction names: It can handle
//  single point events and key events. The event type distinction we removed in
//  CWindowEventHandler and CInputRuleManager re-emerges here.
// Important: This is a pure interface class that does not know anything about the actual event handler.
//   Why is that important? We'll see later...
class IInputRuleEventHandlerConcept
{
public:
    virtual ~IInputRuleEventHandlerConcept() = default;
    // "clone" is a common pattern for base classes to be copyable in some way
    //  It's not possible to just call the copy constructor of a base class, it can't
    //  even know what data members it would need to copy (that depends on the derived class(es)
    // Copyable base classes are dangerous anyway, they invite bugs called "slicing", where
    //  you actually copy only part of an object, the rest is garbage.
    // The clone function will be implemented in each derived class to return a true copy
    //  of that derived class
    virtual std::unique_ptr<IInputRuleEventHandlerConcept> clone() const = 0;
    virtual bool handleSinglePointEvent(const QSinglePointEvent& event, const CInputInfo& inputInfo) = 0;
    virtual bool handleKeyEvent(const QKeyEvent& event, const CInputInfo& inputInfo) = 0;
};


// CInputRuleOwningEventHandlerModel implements our interface class IInputRuleEventHandlerConcept
// It is a template class, because our event handler could be any combination of single point event handler,
//  key event handler or both. That's just three types, but imagine adding touch events, enter/leave events and
//  drag-drop events. That alone would be 31 possible combinations, so it makes sense to write a template for it.
template<class TEventHandler>
class CInputRuleOwningEventHandlerModel : public IInputRuleEventHandlerConcept
{
public:
    CInputRuleOwningEventHandlerModel(TEventHandler eventHandler);
    ~CInputRuleOwningEventHandlerModel() override = default;

    // Here we implement the interface  of IInputRuleEventHandlerConcept
    std::unique_ptr<IInputRuleEventHandlerConcept> clone() const override;
    bool handleSinglePointEvent(const QSinglePointEvent& event, const CInputInfo& inputInfo) override;
    bool handleKeyEvent(const QKeyEvent& event, const CInputInfo& inputInfo) override;

private:
    // Here we store the actual event handler, by value
    TEventHandler m_EventHandler;
};

template<class TEventHandler>
CInputRuleOwningEventHandlerModel<TEventHandler>::CInputRuleOwningEventHandlerModel(TEventHandler eventHandler)
    : m_EventHandler(eventHandler)
{
}

template<class TEventHandler>
std::unique_ptr<IInputRuleEventHandlerConcept> CInputRuleOwningEventHandlerModel<TEventHandler>::clone() const
{
    // Cloning simply creates a new std::unique_ptr with a new CInputRuleOwningEventHandlerModel
    //  The contend is created using the (implicit) copy constructor of CInputRuleOwningEventHandlerModel
    //  Inside that copy constructor, the TEventHandler (m_EventHandler) is copied, too.
    // How can TEventHandler be copyable, although it derives from one or multiple classes?
    //  In this case the inheritance is not an issue: The base classes are just interfaces, which
    //  don't hold data and don't reserve memory. There is also no danger of slicing here, since
    //  we copy using the 'concrete' type TEventHandler, not some base class type
    return std::make_unique<CInputRuleOwningEventHandlerModel>(*this);
}

template<class TEventHandler>
bool CInputRuleOwningEventHandlerModel<TEventHandler>::handleSinglePointEvent(const QSinglePointEvent& event, const CInputInfo& inputInfo)
{
    // So far, all three event handlers we implemented also handles single point events.
    //  But in the future, that might be different. So what do we do?
    // We cannot simply call the event handler function "handleSinglePointEvent", because
    //  it would not compile for types that don't support it.
    // So here is a little trick using C++20 concepts:
    //  We check if the (event handler) type is actually derived from ISinglePointEventHandler,
    //  the interface class for single point events.
    //  If it is, we can call the handleSinglePointEvent function, because the
    //  type must have implemented it to satisfy the ISinglePointEventHandler base class
    //
    // "if constexpr" is a C++17 extension that will be resolved at compile time:
    //  If the expression is true, the code inside the following scope will be compiled into the program
    //  If the expression is false, the code within the following scope will simply not exist in the compiled program
    if constexpr (std::derived_from<TEventHandler, ISinglePointEventHandler>)
    {
        return m_EventHandler.handleSinglePointEvent(event, inputInfo);
    }
    return false;
}
// Alternatively, we could also write a concept that checks if the function
//  "handleSinglePointEvent" with the correct signature exists.
//  I still chose to use an interface class to "tag" the event handler, because
//  that makes it much simpler for the event handler developer: They just have
//  to derive from an interface, implement the virtual function, and things work
//  The interface class is your documentation, and the compiler and linker will help you
//  if you get something wrong.
// If you just write a concept checking the function signature, the event handler writer
//  would have to figure out what exactly they have to provide in order for the concept
//  to be fulfilled, with no guidance:
//  If you implement the wrong function, everything will compile, but it won't work.
// Also, if your argument is "one less virtual function call", then this type erasure
//  implementation (which relies on a virtual function call) is probably not for you.
//  Also, at least in theory, the compiler should be able to figure out that this
//  function call can be devirtualized, because we are running the call on a stored value.
//  Because it's a stored value, m_EventHandler IS EXACTLY TEventHandler, and not some
//  other type further derived.
//  I admit I have no idea which (if any) compiler can actually devirtualize this.
//  Sounds like a nice side project to do on compiler explorer (godbolt.org)

template<class TEventHandler>
bool CInputRuleOwningEventHandlerModel<TEventHandler>::handleKeyEvent(const QKeyEvent& event, const CInputInfo& inputInfo)
{
    if constexpr (std::derived_from<TEventHandler, IKeyEventHandler>)
    {
        return m_EventHandler.handleKeyEvent(event, inputInfo);
    }
    return false;
}

// CInputRulePrivate ties all the parts together:
//  It has a templated constructor, but is itself not a template class
//  The templated constructor is used to create a unique_ptr to a
//  CInputRuleOwningEventHandlerModel (the templated helper class, if you remember)
//  with the actual event handler wrapped inside
// But then, this member is stored as a unique_ptr to IInputRuleEventHandlerConcept,
//  which was the non-templated pure interface class. That way, we basically "forget"
//  the type of TEventHandler on the level of CInputRulePrivate, hence the term "type erasure".
// Through the interface function in IInputRuleEventHandlerConcept, we can still call all the
//  event handler functions, and the actual event handler hidden inside will still do the right thing.
class CInputRulePrivate
{
public:
    template<class TEventHandler>
    CInputRulePrivate(TEventHandler eventHandler, QString ruleId);

    CInputRulePrivate() = default;
    ~CInputRulePrivate() = default;

    static std::unique_ptr<CInputRulePrivate> clone(CInputRulePrivate* const pSource);

    CInputRulePrivate(const CInputRulePrivate& other);
    // It's usually not cool to have a copy constructor, but not a copy assignment operator
    //  That's because the behavior of such a class can be surprising
    // This, however, is a private class that is not meant to be used anywhere but here,
    //  so I can get away with just implementing what is actually needed
    CInputRulePrivate& operator=(const CInputRulePrivate& other) = delete;
    CInputRulePrivate(CInputRulePrivate&& other) = default;
    CInputRulePrivate& operator=(CInputRulePrivate&& other) = default;

public:
    std::unique_ptr<IInputRuleEventHandlerConcept> m_pEventHandlerWrapper{};
    bool m_bDisabled = false;
    QString m_RuleId{"MISSING_ID"};
};

template<class TEventHandler>
CInputRulePrivate::CInputRulePrivate(TEventHandler eventHandler, QString ruleId)
    : m_pEventHandlerWrapper(std::make_unique<CInputRuleOwningEventHandlerModel<TEventHandler>>(std::move(eventHandler))),
      m_RuleId(ruleId)
{
}

#endif // INPUTRULEPRIVATE_H
