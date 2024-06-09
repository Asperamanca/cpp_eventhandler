#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

class QSinglePointEvent;
class QKeyEvent;
class CInputInfo;

//## This interface class now only offers the interface to handle
//  one single event type.
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
class ISinglePointEventHandler
{
public:
    virtual ~ISinglePointEventHandler() = default;
    virtual bool handleSinglePointEvent(const QSinglePointEvent& event, const CInputInfo& inputInfo) = 0;
};

// But how are we going to treat classes that need more than one type of event?
// Let's look at lassoinputrule.h/cpp for an example
class IKeyEventHandler
{
public:
    virtual ~IKeyEventHandler() = default;
    virtual bool handleKeyEvent(const QKeyEvent& event, const CInputInfo& inputInfo) = 0;
};


#endif // EVENTHANDLER_H
