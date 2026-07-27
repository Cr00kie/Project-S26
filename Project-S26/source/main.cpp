#include "SDLApplication.h"

// TODO:
// - Add more element factories to editor
// - Add scripting?

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