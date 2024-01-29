#include "Game.h"

#include <iostream>

#include "Resources.h"

#include "Subsystems/EntityManager.h"
#include "Subsystems/SimulationManager.h"
#include "Scripting/LuaEnvironment.h"

#include "Components/Event.h"

#include "Graphics/Mesh.h"
#include "Graphics/ShaderProgram.h"

int Game::run(const char* title, int width, int height, bool fullscreen)
{
	if (!startup(title, width, height, fullscreen)) {
		std::cout << "Game failed to start up\n";
		return -1;
	}

	//gLuaEnvironment.executeFile("Resources/Scripts/script.lua");

	//auto result = gLuaEnvironment.call("greetings", LuaString::make("C++"), LuaString::make("Lua"));
	//std::cout << getLuaValueString(result) << "\n";

	//auto results = gLuaEnvironment.vectorCall(
	//	"dump_params",
	//	LuaString::make("C++"),
	//	LuaString::make("Lua"),
	//	LuaNumber::make(3.14),
	//	LuaBoolean::make(true),
	//	LuaNil::make());

	//for (const auto& result : results) {
	//	std::cout << getLuaValueString(result) << "\n";
	//}

	//gLuaEnvironment.setTable("position", "x", LuaNumber::make(3));
	//auto x = gLuaEnvironment.getTable("position", "x");
	//auto y = gLuaEnvironment.getTable("position", "y");
	//std::cout << "x=" << std::get<LuaNumber>(x).value << ",y=" << std::get<LuaNumber>(y).value << "\n";

	//gLuaEnvironment.setTable("seq", 1, LuaNumber::make(3));
	//gLuaEnvironment.setTable("seq", 2, LuaNumber::make(9));
	//gLuaEnvironment.setTable("seq", 3, LuaNumber::make(27));

	//auto v1 = gLuaEnvironment.getTable("seq", 1);
	//auto v2 = gLuaEnvironment.getTable("seq", 2);
	//auto v3 = gLuaEnvironment.getTable("seq", 3);
	//std::cout << "seq={"
	//	<< std::get<LuaNumber>(v1).value << ","
	//	<< std::get<LuaNumber>(v2).value << ","
	//	<< std::get<LuaNumber>(v3).value << "}"
	//	<< "\n";

	//gLuaEnvironment.tableCall("Destinations", "new", false, LuaString::make("dst"));
	//gLuaEnvironment.tableCall("dst", "wish", true, LuaString::make("London"), LuaString::make("Paris"), LuaString::make("Amsterdam"));
	//gLuaEnvironment.tableCall("dst", "went", true, LuaString::make("Paris"));
	//auto visited = gLuaEnvironment.vectorTableCall("dst", "list_visited", true);
	//auto unvisited = gLuaEnvironment.vectorTableCall("dst", "list_unvisited", true);

	//std::cout << "Visited:" << "\n";
	//for (auto place : visited) {
	//	std::cout << std::get<LuaString>(place).value << "\n";
	//}

	//std::cout << "Unvisited:" << "\n";
	//for (auto place : unvisited) {
	//	std::cout << std::get<LuaString>(place).value << "\n";
	//}

	gLuaEnvironment.executeFile("Resources/Scripts/cpp_from_lua.lua");

	glEnable(GL_DEPTH_TEST);

	gSimulationManager.createScene();

	float lastFrameTime = 0.0f;
	while (!quit) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			processSDLEvent(event);
		}

		float time = SDL_GetTicks() / 1000.0f;
		float timestep = time - lastFrameTime;
		lastFrameTime = time;

		// FPS Counter
		//static int frameCount = 0;
		//static float totalTime = 0.0f;
		// 
		//totalTime += timestep;
		//frameCount++;

		//if (totalTime >= 1.0f) {
		//	float fps = frameCount / totalTime;
		//	std::cout << "FPS: " << fps << std::endl;
		//	frameCount = 0;
		//	totalTime = 0.0f;
		//}	

		gEntityManager.update(timestep);

		SDL_GL_SwapWindow(window);
	}

	shutdown();

	return 0;
}

bool Game::startup(const char* title, int width, int height, bool fullscreen)
{
	if (!initGame(title, width, height, fullscreen)) {
		return false;
	}

	loadShaderProgram(gResources, "default", "Resources/Shaders/default.vert", "Resources/Shaders/default.frag");
	loadMesh(gResources, "Resources/Meshes/suzanne.obj", "suzanne");
	loadMesh(gResources, "Resources/Meshes/cube.obj", "cube");

	gLuaEnvironment.startUp();
	gEntityManager.startUp();
	gSimulationManager.startUp();

	return true;
}

void Game::shutdown()
{
	gSimulationManager.shutDown();
	gEntityManager.shutDown();
	gLuaEnvironment.shutDown();

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::processSDLEvent(SDL_Event& event)
{
	switch (event.type) {
	case SDL_WINDOWEVENT:
		if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
			int newWidth = event.window.data1;
			int newHeight = event.window.data2;
			glViewport(0, 0, newWidth, newHeight);
		}
		break;
	case SDL_QUIT:
	{
		quit = true;
	}
	break;
	case SDL_KEYDOWN:
	{
		Entity eventEntity = gEntityManager.createEntity();
		gEntityManager.addComponent<KeyEventComponent>(eventEntity, KeyEventComponent(event.key.keysym.sym, true));
	}
	break;
	case SDL_KEYUP:
	{
		Entity eventEntity = gEntityManager.createEntity();
		gEntityManager.addComponent<KeyEventComponent>(eventEntity, KeyEventComponent(event.key.keysym.sym, false));
	}
	break;
	case SDL_MOUSEBUTTONDOWN:
	{
		Entity eventEntity = gEntityManager.createEntity();
		gEntityManager.addComponent<MouseButtonEventComponent>(
			eventEntity,
			MouseButtonEventComponent(event.button.button, event.button.x, event.button.y, true));
	}
	break;
	case SDL_MOUSEBUTTONUP:
	{
		Entity eventEntity = gEntityManager.createEntity();
		gEntityManager.addComponent<MouseButtonEventComponent>(
			eventEntity,
			MouseButtonEventComponent(event.button.button, event.button.x, event.button.y, false));
	}
	break;
	case SDL_MOUSEWHEEL:
	{
	}
	break;
	case SDL_MOUSEMOTION:
	{
		Entity eventEntity = gEntityManager.createEntity();
		gEntityManager.addComponent<MouseMotionEventComponent>(
			eventEntity,
			MouseMotionEventComponent(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel));
	}
	break;
	}
}

bool Game::initGame(const char* title, int width, int height, bool fullscreen)
{
	//-----------------------------------------------------------------------------
	// Initialzie SDL
	//-----------------------------------------------------------------------------
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL error on initialization: " << SDL_GetError() << "\n";
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//-----------------------------------------------------------------------------
	// Create sdl window
	//-----------------------------------------------------------------------------
	const Uint32 windowFlags = (SDL_WINDOW_OPENGL | (fullscreen ? SDL_WINDOW_RESIZABLE : 0));
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
	if (!window) {
		std::cout << "SDL error on create window: " << SDL_GetError() << "\n";
		return false;
	}
	//-----------------------------------------------------------------------------
	// Create opengl context
	//-----------------------------------------------------------------------------
	glContext = SDL_GL_CreateContext(window);
	if (!glContext) {
		std::cout << "SDL GL error on create context: " << SDL_GetError() << "\n";
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}
	//-----------------------------------------------------------------------------
	// Load opengl functions and pointers
	//-----------------------------------------------------------------------------
	if (!gladLoadGL()) {
		std::cout << "GLAD Initialization Error\n";
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	return true;
}