#include "Game.h"

#include "Log/Logger.h"
#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "ECS/Components/Components.h"
#include "ECS/Systems/Systems.h"

Game gGame;

void test() {
	gSystemManager.AddSystem<TestSystem>();

	EntityId entity = gEntityManager.CreateEntity();
	Tag* tagComponent = gComponentManager.AddComponent<Tag>(entity);
}

int Game::Run(const char* title, int width, int height, bool fullscreen)
{
	if (!startup(title, width, height, fullscreen)) {
		spdlog::error("Game start-up failed");
		return -1;
	}

	test();

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

	return true;
}

void Game::shutdown()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
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
		m_quit = true;
	}
	break;
	case SDL_KEYDOWN:
	{

	}
	break;
	case SDL_KEYUP:
	{

	}
	break;
	case SDL_MOUSEBUTTONDOWN:
	{

	}
	break;
	case SDL_MOUSEBUTTONUP:
	{

	}
	break;
	case SDL_MOUSEWHEEL:
	{
	}
	break;
	case SDL_MOUSEMOTION:
	{

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