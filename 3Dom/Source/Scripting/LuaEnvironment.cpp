#include "LuaEnvironment.h"

#include <iostream>
#include <vector>
#include <functional>

#include "ECS/EntityManager.h"

LuaEnvironment gLuaEnvironment;

void LuaEnvironment::startUp()
{
	m_luaState = sol::state();
	m_luaState.open_libraries(sol::lib::base, sol::lib::package);
}

void LuaEnvironment::shutDown()
{
}

void LuaEnvironment::execute(const std::string& filepath)
{
	try {
		m_luaState.script_file(filepath);
	}
	catch (const sol::error& e) {
		std::cerr << "Failed to execute script: " << e.what() << std::endl;
	}
}
