#include "LuaEnvironment.h"

#include <iostream>
#include <cassert>

LuaEnvironment gLuaEnvironment;

void LuaEnvironment::startUp()
{
	L = luaL_newstate();
	luaL_openlibs(L);
	std::cout << "Lua version number is " << lua_version(L) << "\n";
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

LuaValue LuaEnvironment::call(const std::string& function, const LuaValue& param)
{
	int type = lua_getglobal(L, function.c_str());
	assert(LUA_TFUNCTION == type);
	pushValue(param);
	pcall(1, 1);
	return popValue();
}

void LuaEnvironment::pcall(int nargs, int nresults)
{
	if (lua_pcall(L, nargs, nresults, 0)) {
		std::cerr << "Failed to execute Lua code:" << popString() << "\n";
	}
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

