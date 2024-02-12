#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

#include <string>

#include <sol/sol.hpp>

#include "Event/LuaEvent.h"
#include "Input/InputManager.h"

struct UpdateEvent {
	float m_timestep;
};

class ScriptManager {
public:
	void StartUp();
	void ShutDown();

	void Execute(const std::string& filepath);

	LuaEvent m_luaUpdateEvent;
private:
	void onUpdate(const UpdateEvent& event);
private:
	sol::state m_lua;
};

extern ScriptManager gScriptManager;

#endif 