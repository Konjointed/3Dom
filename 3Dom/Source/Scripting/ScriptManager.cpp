#include "ScriptManager.h"

#include "Log/Logger.h"
#include "Event/Event.h"
#include "Event/EventManager.h"
#include "Input/InputManager.h"

ScriptManager gScriptManager;

void ScriptManager::StartUp()
{

	m_lua = sol::state();
	m_lua.open_libraries(sol::lib::base, sol::lib::package);

	m_lua.new_usertype<Event>("Event",
		"Connect", &Event::Connect
		);

	m_lua.new_usertype<InputManager>("InputManager",
		"Update", &InputManager::m_luaUpdateEvent,
		"KeyPress", &InputManager::m_luaKeyPressEvent,
		"KeyRelease", &InputManager::m_luaKeyReleaseEvent,

		"IsKeyDown", &InputManager::IsKeyDown,
		"IsMouseButtonDown", &InputManager::IsMouseButtonDown,
		"DeltaMouseX", &InputManager::DeltaMouseX,
		"DeltaMouseY", &InputManager::DeltaMouseY
	);

	m_lua["input"] = &gInputManager;

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
