#ifndef LUA_ENVIRONMENT_H
#define LUA_ENVIRONMENT_H

#include <vector>
#include <string>
#include <cassert>

#include "LuaType.h"

extern "C" {
	#include "lua/lua.h"
	#include "lua/lualib.h"
	#include "lua/lauxlib.h"
}

class LuaEnvironment {
public:
	static const int versionCode = 6;

	LuaEnvironment() = default;
	~LuaEnvironment() = default;

	void startUp();
	void shutDown();

	void executeFile (const std::string& filepath);
	void execute(const std::string& script);

	std::string getGlobalString(const std::string& name);
	void setGlobalString(const std::string& name, const std::string& value);

	LuaValue getTable(const std::string & table, const std::string& key);
	void setTable(const std::string& table, const std::string& key, const LuaValue& value);
	LuaValue getTable(const std::string& table, int index);
	void setTable(const std::string& table, int index, const LuaValue& value);

	template <typename... Ts>
	LuaValue call(const std::string& function, const Ts&...params);

	template <typename... Ts>
	std::vector<LuaValue> vectorCall(const std::string& function, const Ts&...params);

	template <typename... Ts>
	LuaValue tableCall(const std::string& table, const std::string& function, bool shouldPassSelf, const Ts&...params);

	template <typename... Ts>
	std::vector<LuaValue> vectorTableCall(const std::string& table, const std::string& function, bool shouldPassSelf, const Ts&...params);
private:
	bool pcall(int nargs = 0, int nresults = 0);
	std::string popString();
	void pushValue(const LuaValue& value);
	LuaValue getValue(int index);
	LuaValue popValue();
	std::vector<LuaValue> popValues(int n);
private:
	lua_State* L;
};

template <typename... Ts>
LuaValue LuaEnvironment::call(const std::string& function, const Ts&...params) {
	int type = lua_getglobal(L, function.c_str());
	assert(LUA_TFUNCTION == type);
	for (auto param : std::initializer_list<LuaValue>{ params... }) {
		pushValue(param);
	}
	pcall(sizeof...(params), 1);
	return popValue();
}

template <typename... Ts>
std::vector<LuaValue> LuaEnvironment::vectorCall(const std::string& function, const Ts&...params) {
	int stackSz = lua_gettop(L);
	int type = lua_getglobal(L, function.c_str());
	assert(LUA_TFUNCTION == type);
	for (auto param : std::initializer_list<LuaValue>{ params... }) {
		pushValue(param);
	}
	if (pcall(sizeof...(params), LUA_MULTRET)) {
		int nresults = lua_gettop(L) - stackSz;
		return popValues(nresults);
	}
	return std::vector<LuaValue>();
}

template <typename... Ts>
LuaValue LuaEnvironment::tableCall(const std::string& table, const std::string& function, bool shouldPassSelf, const Ts&...params) {
	int type = lua_getglobal(L, table.c_str());
	assert(LUA_TTABLE == type);

	type = lua_getfield(L, -1, function.c_str());
	assert(LUA_TFUNCTION == type);

	if (shouldPassSelf)
	{
		lua_getglobal(L, table.c_str());
	}

	for (const auto& param : std::initializer_list<LuaValue>{ params... })
	{
		pushValue(param);
	}

	int nparams = sizeof...(params) + (shouldPassSelf ? 1 : 0);
	pcall(nparams, 1);

	auto result = popValue();
	lua_pop(L, 1);

	return result;
}

template <typename... Ts>
std::vector<LuaValue> LuaEnvironment::vectorTableCall(const std::string& table, const std::string& function, bool shouldPassSelf, const Ts&...params) {
	int type = lua_getglobal(L, table.c_str());
	assert(LUA_TTABLE == type);

	int stackSz = lua_gettop(L);

	type = lua_getfield(L, -1, function.c_str());
	assert(LUA_TFUNCTION == type);

	if (shouldPassSelf)
	{
		lua_getglobal(L, table.c_str());
	}

	for (const auto& param : std::initializer_list<LuaValue>{ params... })
	{
		pushValue(param);
	}

	auto results = std::vector<LuaValue>();
	int nparams = sizeof...(params) + (shouldPassSelf ? 1 : 0);
	if (pcall(nparams, LUA_MULTRET))
	{
		int nresults = lua_gettop(L) - stackSz;
		results = popValues(nresults);
	}
	lua_pop(L, 1);
	return results;
}

extern LuaEnvironment gLuaEnvironment;

#endif 