#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

class QSinglePointEvent;
class QKeyEvent;
class CInputInfo;

//## This interface class now only offers the interface to handle
//  one single event type. This is perfect for a class that only
//  wants to handle a single event type anyway (see dragmoveeventhandler.h/cpp)
class ISinglePointEventHandler
{
public:
    virtual ~ISinglePointEventHandler() = default;
    virtual bool handleSinglePointEvent(const QSinglePointEvent& event, const CInputInfo& inputInfo) = 0;
};

// But how are we going to treat classes that need more than one type of event?
// Let's look at lassoeventhandler.h/cpp for an example
class IKeyEventHandler
{
public:
    virtual ~IKeyEventHandler() = default;
    virtual bool handleKeyEvent(const QKeyEvent& event, const CInputInfo& inputInfo) = 0;
};


#endif // EVENTHANDLER_H
