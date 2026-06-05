#include "SDLApplication.h"
#include <SDL3/SDL_main.h>

// TODO:
// - Add more element factories to editor
// - Add scripting?
// - Add Debug/Logging class

int main(int argc, char* argv[])
{
	SDLApplication game;

	if (!game.init())
	{
		return 1;
	}

	game.run();

	return 0;
}