#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "EntityManager.h"
#include "SimulationManager.h"

class Game {
public:
	static Game& get() {
		static Game instance;
		return instance;
	}

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	int run(const char* title, int width, int height, bool fullscreen);

	EntityManager& getEntityManager() {
		return m_entityManager;
	}

	SimulationManager& getSimulationManager() {
		return m_simulationManager;
	}
private:
	Game() = default;

	bool startup(const char* title, int width, int height, bool fullscreen);
	void shutdown();

	void processSDLEvent(SDL_Event& event);

	// initializes SDL and OpenGL
	bool initGame(const char* title, int width, int height, bool fullscreen);
private:
	bool quit = false;
	SDL_Window* window = nullptr;
	SDL_GLContext glContext = nullptr;

	EntityManager m_entityManager;
	SimulationManager m_simulationManager;
};

#endif 