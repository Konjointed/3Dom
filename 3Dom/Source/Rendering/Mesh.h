#include "Game.h"

#include "Resources.h"

#include "Log/Logger.h"
#include "Core/Resources.h"
#include "Scripting/ScriptManager.h"
#include "Input/InputManager.h"
#include "Event/EventManager.h"
#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "Rendering/RenderingPipeline.h"
#include "Rendering/Mesh.h"
#include "Rendering/ShaderProgram.h"

Game gGame;

int Game::Run(const char* title, int width, int height, bool fullscreen)
{
	if (!startup(title, width, height, fullscreen, gResources)) {
		spdlog::error("Game start-up failed");
		return -1;
	}

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
		//static int frameCoun