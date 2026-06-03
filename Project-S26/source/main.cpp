#include "SDLApplication.h"
#include <SDL3/SDL_main.h>

// TODO:
// - Make logic for interactive flag in UIElement
// - Combine Image, Nineslice and Rect into UIElement
// - Add Trickle down and bubble up logic to UIElement EventHandling
// - Implement Scene builder to create scenes from Tiled (Take and adapt previous implementation)

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