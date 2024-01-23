#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>

class Game {
public:
	static Game& get() {
		static Game instance;
		return instance;
	}

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	int run(const char* title, int width, int height, bool fullscreen);
private:
	Game() = default;

	bool startup(const char* title, int width, int height, bool fullscreen);
	void shutdown();
	void processSDLEvent(SDL_Event& event);

	bool loadSDL(const char* title, int width, int height, bool fullscreen);
	bool loadResources();
	bool loadSubsystems();
private:
	bool quit = false;
	SDL_Window* window = nullptr;
	SDL_GLContext glContext = nullptr;
};

#endif 