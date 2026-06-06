#include "SDLApplication.h"
#include <SDL3/SDL_main.h>

// TODO:
// - Add more element factories to editor
// - Add scripting?
// - Create a Slider
// - Make scene manager support having more than one scene at the same time (Have per scene, isUpdating and isRendering)

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