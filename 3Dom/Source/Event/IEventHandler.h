#ifndef IEVENT_HANDLER_H
#define IEVENT_HANDLER_H

class IEvent;

class IEventHandler {
public:
    virtual ~IEventHandler() = default;
    virtual void OnEvent(IEvent& event) = 0;
};

#endif 
