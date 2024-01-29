#ifndef LUA_ENVIRONMENT_H
#define LUA_ENVIRONMENT_H

#include <sol/sol.hpp>

class LuaEnvironment {
public:
	LuaEnvironment() = default;
	~LuaEnvironment() = default;

	void startUp();
	void shutDown();
private:
	sol::state m_luaState;
};

extern LuaEnvironment gLuaEnvironment;

#endif 