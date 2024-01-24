#include <iostream>

#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>

struct Resources {

} gResources;

struct ApplicationCtx {
	bool quit = false;
	SDL_Window* window = nullptr;
	SDL_GLContext glContext = nullptr;
} gContext;

bool loadSDL(ApplicationCtx& context, const char* title, int width, int height, bool fullscreen);
bool initializeGame(ApplicationCtx& context, Resources& resources);

int main(int argc, char* argv[]) {
    if (!initializeGame(gContext, gResources)) {
        return -1;
    }

    // Startup subsystems

    // Run game

    // Shutdown subsystems in reverse order

    return 0;
}

bool initializeGame(ApplicationCtx& context, Resources& resources) {
	if (!loadSDL(context, "My Application", 1280, 720, false)) {
		std::cerr << "Failed to initialize SDL and OpenGL" << std::endl;
        return false;
	}
}

bool loadSDL(ApplicationCtx& context, const char* title, int width, int height, bool fullscreen) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL error on initialization: " << SDL_GetError() << "\n";
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create SDL window
    const Uint32 windowFlags = (SDL_WINDOW_OPENGL | (fullscreen ? SDL_WINDOW_RESIZABLE : 0));
    context.window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
    if (!context.window) {
        std::cout << "SDL error on create window: " << SDL_GetError() << "\n";
        return false;
    }

    // Create OpenGL context
    context.glContext = SDL_GL_CreateContext(context.window);
    if (!context.glContext) {
        std::cout << "SDL GL error on create context: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(context.window);
        SDL_Quit();
        return false;
    }

    // Load OpenGL functions and pointers
    if (!gladLoadGL()) {
        std::cout << "GLAD Initialization Error\n";
        SDL_GL_DeleteContext(context.glContext);
        SDL_DestroyWindow(context.window);
        SDL_Quit();
        return false;
    }

    return true;
}