#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <functional>

#include <sol/sol.hpp> 

/* TODO:
- Rename this class
    - LuaEventBinder
    - ScriptEventBinder
    - EventProxy
    - CallbackBinder
    - LuaCallbackHandler
*/
class Event {
public:
    void Connect(sol::function callback) {
        listeners.push_back(callback);
    }

    template<typename... Args>
    void Trigger(Args... args) {
        for (auto& listener : listeners) {
            listener(args...);
        }
    }

private:
    std::vector<sol::function> listeners;
};

#endif 