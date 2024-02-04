#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

#include <string>

#include <sol/sol.hpp>

class ScriptManager {
public:
	void StartUp();
	void ShutDown();

	void Execute(const std::string& filepath);
private:
	sol::state m_lua;
};

extern ScriptManager gScriptManager;

#endif 