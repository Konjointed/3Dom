#ifndef LUA_ENVIRONMENT_H
#define LUA_ENVIRONMENT_H

#include <vector>
#include <string>
#include <variant>
#include <cassert>

extern "C" {
	#include "lua/lua.h"
	#include "lua/lualib.h"
	#include "lua/lauxlib.h"
}

// TODO: Move to LuaType.h
enum class LuaType {
	nil,
	boolean,
	number,
	string,
};

struct LuaNil final {
	const LuaType type = LuaType::nil;
	const std::nullptr_t value = nullptr;
	static LuaNil make() { return LuaNil(); }
private:
	LuaNil() = default;
};

struct LuaBoolean final {
	const LuaType type = LuaType::boolean;
	const bool value;
	static LuaBoolean make(const bool value) {
		return LuaBoolean(value);
	}
private:
	LuaBoolean(const bool value) : value(value) {}
};

struct LuaNumber final {
	const LuaType type = LuaType::number;
	const double value;
	static LuaNumber make(const double value) {
		return LuaNumber(value);
	}
private:
	LuaNumber(const double value) : value(value) {}
};

struct LuaString final {
	const LuaType type = LuaType::string;
	const std::string value;
	static LuaString make(const std::string& value) {
		return LuaString(value);
	}
private:
	LuaString(const std::string& value) : value(value) {}
};

// Creates a type alias "LuaValue" which can represent any of the four types
using LuaValue = std::variant<LuaNil, LuaBoolean, LuaNumber, LuaString>;

inline LuaType getLuaType(const LuaValue& value) {
	return std::visit([](const auto& v) { return v.type; }, value);
}

inline std::string getLuaValueString(const LuaValue& value) {
	switch (getLuaType(value)) {
	case LuaType::nil:
		return "nil";
	case LuaType::boolean:
		return std::get<LuaBoolean>(value).value ? "true" : "false";
	case LuaType::number:
		return std::to_string(std::get<LuaNumber>(value).value);
	case LuaType::string:
		return std::get<LuaString>(value).value;
	}
}

class LuaEnvironment {
public:
	LuaEnvironment() = default;
	~LuaEnvironment() = default;

	void startUp();
	void shutDown();

	void executeFile (const std::string& filepath);
	void execute(const std::string& script);

	std::string getGlobalString(const std::string& name);
	void setGlobalString(const std::string& name, const std::string& value);

	template <typename... Ts>
	LuaValue call(const std::string& function, const Ts&...params);

	template <typename... Ts>
	std::vector<LuaValue> vectorCall(const std::string& function, const Ts&...params);
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

extern LuaEnvironment gLuaEnvironment;

#endif 