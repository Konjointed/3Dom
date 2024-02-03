#ifndef IEVENT_H
#define IEVENT_H

enum class EventType 
{
	EVENT_NONE,
	EVENT_KEY_PRESSED,
	EVENT_KEY_RELEASED,
	EVENT_MOUSE_BUTTON_PRESSED,
	EVENT_MOUSE_BUTTON_RELEASED,
	EVENT_MOUSE_MOVE,
	EVENT_MOUSE_SCROLL
};

class IEvent
{
public:
	virtual ~IEvent() = default;
	virtual EventType GetStaticType() const = 0;
};

#endif 