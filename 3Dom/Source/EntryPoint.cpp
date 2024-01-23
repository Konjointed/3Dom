#include <iostream>

#include "Game.h"

int main(int argc, char* argv[]) {
	Game& game = Game::get();
	return game.run("Graphics Engine", 1280, 720, true);
}