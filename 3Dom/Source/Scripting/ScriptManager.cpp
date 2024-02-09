#include "ScriptManager.h"

#include "Log/Logger.h"
#include "Event/LuaEvent.h"
#include "Event/EventManager.h"
#include "Input/InputManager.h"

ScriptManager gScriptManager;

void ScriptManager::StartUp()
{
	gEventManager.Connect<UpdateEvent>(this, &ScriptManager::onUpdate);

	m_lua = sol::state();
	m_lua.open_libraries(sol::lib::base, sol::lib::package);

	m_lua.new_usertype<LuaEvent>("LuaEvent",
		"Connect", &LuaEvent::operator+=
		);

	m_lua["game"] = &gScriptManager;
	m_lua.new_usertype<ScriptManager>("ScriptManager",
		"Update", &ScriptManager::m_luaUpdateEvent
		);

	m_lua["input"] = &gInputManager;
	m_lua.new_usertype<InputManager>("InputManager",
		"KeyPress", &InputManager::m_luaKeyPressEvent,
		"KeyRelease", &InputManager::m_luaKeyReleaseEvent,

		"IsKeyDown", &InputManager::IsKeyDown,
		"IsMouseButtonDown", &InputManager::IsMouseButtonDown,
		"GetMouseDelta", &InputManager::GetMouseDelta
	);

	Execute("Resources/Scripts/script.lua");
}

void ScriptManager::ShutDown()
{
}

void ScriptManager::Execute(const std::string& filepath)
{
	try {
		m_lua.script_file(filepath);
	}
	catch (const sol::error& e) {
		spdlog::error("ERROR::SCRIPTMANAGER: Failed to execute script {}", e.what());
	}
}

void ScriptManager::onUpdate(const UpdateEvent& event)
{
	m_luaUpdateEvent(event.m_timestep);
}
