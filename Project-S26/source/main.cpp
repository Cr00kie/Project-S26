#include "SDLApplication.h"
#include <SDL3/SDL_main.h>

// TODO:
// - Combine Image, Nineslice and Rect into a single Sprite class (don't put it all in UIElement, kinda sucks)
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