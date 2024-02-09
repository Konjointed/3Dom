#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <functional>

#include <sol/sol.hpp> 

/*
This class is exposed to lua using sol2 so that you can connect to events.

Example:
input.KeyPress:Connect(function(key)
    --print("Key pressed: " .. key)
end)

Event handlers will have to call Trigger() so that the lua code also receives the event

Example:
LuaEvent m_luaKeyPressEvent;

// This is the actual event
void SomeClass::onKeyPressed(const KeyPressEvent& event)
{
    // Do something with the event

    // Trigger the lua event so that lua code is notified about this
    m_luaKeyPressEvent(event.m_keycode);
}
*/
class LuaEvent {
public:
    template<typename... Args>
    void operator()(Args... args) {
        for (auto& listener : listeners) {
            listener(args...);
        }
    }

    void operator+=(sol::function callback) {
        listeners.push_back(callback);
    }
private:
    std::vector<sol::function> listeners;
};

#endif 