#pragma once
#include "AppFacadeService.h"
#include "SDLApplication.h"

AppFacadeService::AppFacadeService(SDLApplication* app)
:app(app){
}

void
AppFacadeService::quit() {
	app->m_bIsRunning = false;
}
SDL_Renderer&
AppFacadeService::getRenderer() {
	return app->getRenderer();
}
int
AppFacadeService::getResolutionW() {
	return app->M_RESOLUTIONWIDTH;
}
int
AppFacadeService::getResolutionH() {
	return app->M_RESOLUTIONHEIGHT;
}
int
AppFacadeService::getFPS() {
	return app->M_IFPS;
}
SDL_Window* 
AppFacadeService::getWindow() {
	return app->m_pWindow;
}