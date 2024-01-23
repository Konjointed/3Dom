#include "Game.h"

#include <iostream>

int Game::run(const char* title, int width, int height, bool fullscreen)
{
	if (!startup(title, width, height, fullscreen)) {
		std::cout << "Game failed to initialize\n";
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	float lastFrameTime = 0.0f;
	while (!quit) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			processSDLEvent(event);
		}

		float time = SDL_GetTicks() / 1000.0f;
		float timestep = time - lastFrameTime;
		lastFrameTime = time;

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

		SDL_GL_SwapWindow(window);
	}

	shutdown();

	return 0;
}

bool Game::startup(const char* title, int width, int height, bool fullscreen)
{
	return loadSDL(title, width, height, fullscreen) &&
		loadResources() &&
		loadSubsystems() &&
		loadSystems();
}

void Game::shutdown()
{
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
		//KeyboardPressEvent evt(event.key.keysym.sym);
		//dispatcher->Dispatch(evt);
	}
	break;
	case SDL_KEYUP:
	{
		//KeyboardReleaseEvent evt(event.key.keysym.sym);
		//dispatcher->Dispatch(evt);
	}
	break;
	case SDL_MOUSEBUTTONDOWN:
	{
		//MouseButtonPressEvent evt(event.button.button);
		//dispatcher->Dispatch(evt);
	}
	break;
	case SDL_MOUSEBUTTONUP:
	{
		//MouseButtonReleaseEvent evt(event.button.button);
		//dispatcher->Dispatch(evt);
	}
	break;
	case SDL_MOUSEWHEEL:
	{
	}
	break;
	case SDL_MOUSEMOTION:
	{
		//MouseMoveEvent evt(event.motion.x, event.motion.y);
		//dispatcher->Dispatch(evt);
	}
	break;
	}
}

bool Game::loadSDL(const char* title, int width, int height, bool fullscreen)
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

bool Game::loadResources()
{
	return true;
}

bool Game::loadSubsystems()
{
	return true;
}

bool Game::loadSystems()
{
	return true;
}
