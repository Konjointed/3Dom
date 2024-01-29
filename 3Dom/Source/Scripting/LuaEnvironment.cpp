#include "LuaEnvironment.h"

#include <iostream>

LuaEnvironment gLuaEnvironment;

class TestClass {
public:
	void doSomething() {
		std::cout << "Doing something interest\n";
	}
	void doSomethingElse() {
		std::cout << "Doing something else that's interesting\n";
	}
private:
};

void LuaEnvironment::startUp()
{
	m_luaState = sol::state();
	m_luaState.open_libraries(sol::lib::base, sol::lib::package);

	m_luaState.new_usertype<TestClass>("TestClass",
			"doSomething", &TestClass::doSomething,
			"doSomethingElse", &TestClass::doSomethingElse
		);

	TestClass testClass;
	m_luaState["test"] = &testClass;

	try {
		m_luaState.script_file("Resources/Scripts/script.lua");
	}
	catch (const sol::error& e) {
		std::cerr << "Failed to execute script: " << e.what() << std::endl;
	}
}

void LuaEnvironment::shutDown()
{
}
