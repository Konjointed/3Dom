#include "LuaEnvironment.h"

#include <iostream>

namespace {
	int luaGetExecutorVersionCode(lua_State* L) {
		lua_pushinteger(L, LuaEnvironment::versionCode);
		return 1;
	}

	void registerHostFunctions(lua_State* L) {
		lua_pushcfunction(L, luaGetExecutorVersionCode);
		lua_setglobal(L, "host_version");
	}
}

LuaEnvironment gLuaEnvironment;

void LuaEnvironment::startUp()
{
	L = luaL_newstate();
	std::cout << "Lua version number is " << lua_version(L) << "\n";
	luaL_openlibs(L);

	registerHostFunctions(L);
}

void LuaEnvironment::shutDown()
{
	lua_close(L);
}

void LuaEnvironment::executeFile(const std::string& filepath)
{
	if (luaL_loadfile(L, filepath.c_str())) {
		std::cerr << "Failed to prepare file:" << popString() << "\n";
		return;
	}
	pcall();
}

void LuaEnvironment::execute(const std::string& script)
{
	if (luaL_loadstring(L, script.c_str())) {
		std::cerr << "Failed to prepare script:" << popString() << "\n";
		return;
	}
	pcall();
}

std::string LuaEnvironment::getGlobalString(const std::string& name)
{
	const int type = lua_getglobal(L, name.c_str());
	assert(LUA_TSTRING == type);
	return popString();
}

void LuaEnvironment::setGlobalString(const std::string& name, const std::string& value)
{
	lua_pushstring(L, value.c_str());
	lua_setglobal(L, name.c_str());
}

LuaValue LuaEnvironment::getTable(const std::string& table, const std::string& key)
{
	int type = lua_getglobal(L, table.c_str());
	assert(LUA_TTABLE == type);
	lua_pushstring(L, key.c_str());
	lua_gettable(L, -2);
	auto value = popValue();
	lua_pop(L, 1);
	return value;
}

void LuaEnvironment::setTable(const std::string& table, const std::string& key, const LuaValue& value)
{
	int type = lua_getglobal(L, table.c_str());
	assert(LUA_TTABLE == type);
	lua_pushstring(L, key.c_str());
	pushValue(value);
	lua_settable(L, -3);
	lua_pop(L, 1);
}

LuaValue LuaEnvironment::getTable(const std::string& table, int index)
{
	int type = lua_getglobal(L, table.c_str());
	assert(LUA_TTABLE == type);
	lua_geti(L, -1, index);
	auto value = popValue();
	lua_pop(L, 1);
	return value;
}

void LuaEnvironment::setTable(const std::string& table, int index, const LuaValue& value)
{
	int type = lua_getglobal(L, table.c_str());
	assert(LUA_TTABLE == type);
	pushValue(value);
	lua_seti(L, -2, index);
	lua_pop(L, 1);
}

bool LuaEnvironment::pcall(int nargs, int nresults)
{
	if (lua_pcall(L, nargs, nresults, 0)) {
		std::cerr << "Failed to execute Lua code:" << popString() << "\n";
		return false;
	}
	return true;
}

std::string LuaEnvironment::popString()
{
	auto result = std::get<LuaString>(popValue());
	return result.value;
}

void LuaEnvironment::pushValue(const LuaValue& value)
{
	switch (getLuaType(value)) {
	case LuaType::nil:
		lua_pushnil(L);
		break;
	case LuaType::boolean:
		lua_pushboolean(L, std::get<LuaBoolean>(value).value ? 1 : 0);
		break;
	case LuaType::number:
		lua_pushnumber(L, std::get<LuaNumber>(value).value);
		break;
	case  LuaType::string:
		lua_pushstring(L, std::get<LuaString>(value).value.c_str());
		break;
	}
}

LuaValue LuaEnvironment::getValue(int index)
{
	switch (lua_type(L, index)) {
	case LUA_TNIL:
		return LuaNil::make();
	case LUA_TBOOLEAN:
		return LuaBoolean::make(lua_toboolean(L, index) == 1);
	case LUA_TNUMBER:
		return LuaNumber::make((double)lua_tonumber(L, index));
	case LUA_TSTRING:
		return LuaString::make(lua_tostring(L, index));
	default:
		return LuaNil::make();
	}
}

LuaValue LuaEnvironment::popValue()
{
	auto value = getValue(-1);
	lua_pop(L, 1);
	return value;
}

std::vector<LuaValue> LuaEnvironment::popValues(int n)
{
	std::vector<LuaValue> results;
	for (int i = n; i > 0; --i) {
		results.push_back(getValue(-i));
	}
	lua_pop(L, n);
	return results;
}

