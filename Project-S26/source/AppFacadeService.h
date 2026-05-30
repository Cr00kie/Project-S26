#pragma once

#include "SDL3/SDL.h"

class SDLApplication;
class Event;

//Allow the main application to be accessed by the different layers and managers
//without creating a dependency on the SDLApplication class
class AppFacadeService {
private:
	//Pointer to the main application, used to call its methods
	SDLApplication* app;
public:
	// Creates an AppFacadeService which provides an interface to the app
	// Arguments:
	//		- app: The app for which the AppFacadeService will expose the functions
	AppFacadeService(SDLApplication* app);

	//Close the application
	void quit();

	//Returns a reference to the SDL_Renderer of the application
	SDL_Renderer& getRenderer();

	//Returns the width of the application window
	int getResolutionW();

	//Returns the height of the application window
	int getResolutionH();

	//Returns the target FPS of the application
	int getFPS();

	SDL_Window* getWindow();
};