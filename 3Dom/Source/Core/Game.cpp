#include "Game.h"

#include "Resources.h"

#include "Log/Logger.h"
#include "Scripting/ScriptManager.h"
#include "Input/InputManager.h"
#include "Event/EventManager.h"
#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"

#include "Rendering/Mesh.h"
#include "Rendering/ShaderProgram.h"

Game gGame;

int Game::Run(const char* title, int width, int height, bool fullscreen)
{
	if (!startup(title, width, height, fullscreen)) {
		spdlog::error("Game start-up failed");
		return -1;
	}

	CreateScene();

	glEnable(GL_DEPTH_TEST);

	float lastFrameTime = 0.0f;
	while (!m_quit) {
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

		gSystemManager.Update(timestep);
		gInputManager.Update();

		gEventManager.Fire<UpdateEvent>(timestep);

		SDL_GL_SwapWindow(m_window);
	}

	shutdown();

	return 0;
}

bool Game::startup(const char* title, int width, int height, bool fullscreen)
{
	if (!initGame(title, width, height, fullscreen)) {
		return false;
	}

	gScriptManager.StartUp();
	gInputManager.StartUp();

	loadResources(gResources);

	return true;
}

void Game::shutdown()
{
	gInputManager.ShutDown();
	gScriptManager.ShutDown();

	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Game::processSDLEvent(SDL_Event& event)
{
	switch (event.type) 
	{
		case SDL_WINDOWEVENT:
		{
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				int newWidth = event.window.data1;
				int newHeight = event.window.data2;
				glViewport(0, 0, newWidth, newHeight);
			}
			break;
		}
		case SDL_QUIT:
		{
			m_quit = true;
			break;
		}
		case SDL_KEYDOWN:
		{
			gEventManager.Fire<KeyPressEvent>(event.key.keysym.sym);
			break;
		}
		case SDL_KEYUP:
		{
			gEventManager.Fire<KeyReleaseEvent>(event.key.keysym.sym);
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			gEventManager.Fire<ButtonPressEvent>(event.button.button);
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			gEventManager.Fire<ButtonReleaseEvent>(event.button.button);
			break;
		}
		case SDL_MOUSEWHEEL:
		{
			break;
		}
		case SDL_MOUSEMOTION:
		{
			gEventManager.Fire<MouseMoveEvent>(event.motion.x, event.motion.y);
			break;
		}
	}
}

bool Game::initGame(const char* title, int width, int height, bool fullscreen)
{
	//-----------------------------------------------------------------------------
	// Initialzie SDL
	//-----------------------------------------------------------------------------
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		spdlog::error("SDL Init {}", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//-----------------------------------------------------------------------------
	// Create sdl window
	//-----------------------------------------------------------------------------
	const Uint32 windowFlags = (SDL_WINDOW_OPENGL | (fullscreen ? SDL_WINDOW_RESIZABLE : 0));
	m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
	if (!m_window) {
		spdlog::error("SDL Create Window {}", SDL_GetError());
		return false;
	}
	//-----------------------------------------------------------------------------
	// Create opengl context
	//-----------------------------------------------------------------------------
	m_glContext = SDL_GL_CreateContext(m_window);
	if (!m_glContext) {
		spdlog::error(" SDL GL Context {}", SDL_GetError());
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}
	//-----------------------------------------------------------------------------
	// Load opengl functions and pointers
	//-----------------------------------------------------------------------------
	if (!gladLoadGL()) {
		spdlog::error("GLAD Init {}", SDL_GetError());
		SDL_GL_DeleteContext(m_glContext);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	return true;
}

void Game::loadResources(Resources& resources)
{
	LoadShaderProgram(resources, "default", "Resources/Shaders/default.vert", "Resources/Shaders/default.frag");
	LoadMesh(resources, "Resources/Meshes/cube.obj", "cube");
	LoadMesh(resources, "Resources/Meshes/suzanne.obj", "suzanne");
}