#ifndef EVENT_DISPATCHER
#define EVENT_DISPATCHER

#include <vector>

class IEventHandler;
class IEvent;

class EventDispatcher {
public:
	void Connect(IEventHandler* handler);
	void Disconnect(IEventHandler* handler);
	void Dispatch(IEvent& event);
private:
	std::vector<IEventHandler*> handlers;
};

#endif 
