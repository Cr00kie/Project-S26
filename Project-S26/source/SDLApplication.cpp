#include "SDLApplication.h"
#include "Resources/ResourceManager.h"
#include "Events/EventBus.h"
#include "Tweens/TweenManager.h"
#include "Audio/AudioManager.h"
#include "Scenes/SceneManager.h"

#include <SDL3/SDL.h>
#include <memory>
#include <tuple>

bool SDLApplication::init()
{
	// Initialize SDL
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Init Error", SDL_GetError(), m_pWindow);
		return false;
	}

	// Get Device size
	const SDL_DisplayMode* dm = SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay());
	int w, h;
	// If no device size, set 1080p as default
	w = dm ? dm->w : 1920;
	h = dm ? dm->h : 1080;

	// Create window
	m_pWindow = SDL_CreateWindow(
		"Project S26 - The Last Braincell",
#if _DEBUG
		// Make screen slightly smaller when debugging
		int(w / 1.5f),
		int(h / 1.5f),
#else
		// Make screen fullscreen when on release
		w,
		h,
#endif
		SDL_WINDOW_RESIZABLE
	);

	if (!m_pWindow) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateWindow Error", SDL_GetError(), m_pWindow);
		return false;
	}

	// Create renderer
	m_pRenderer = SDL_CreateRenderer(m_pWindow, NULL);

	if (!m_pRenderer) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateRenderer Error", SDL_GetError(), m_pWindow);
		return false;
	}

	SDL_SetRenderVSync(m_pRenderer, 1);

	// Initialize TTF_Library
	if (!TTF_Init())
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "TTF Init Error", SDL_GetError(), m_pWindow);
		return false;
	}

	//Initialize audio
	MIX_Init();
	m_sfxMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
	m_musicMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

	if (!m_sfxMixer || !m_musicMixer)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Mixer init error", SDL_GetError(), m_pWindow);
		return false;
	}

	// Initialize global services
	initGlobalServices();

	// Config render
	SDL_SetRenderLogicalPresentation(m_pRenderer, 1920, 1080,
		SDL_RendererLogicalPresentation::SDL_LOGICAL_PRESENTATION_LETTERBOX);

	// Subscribe quit event
	EventBus* eb = ServiceLocator::tryGet<EventBus>();
	if (eb != nullptr) {
		eb->subscribe<WindowEvent>("Window_Quitted", [this](const Event&) {
			m_bIsRunning = false;
			});
	}

	m_bIsRunning = true;
	return true;
}

void SDLApplication::run()
{
	uint64_t lastTime = SDL_GetTicks();
	while (m_bIsRunning)
	{
		uint64_t currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f;
		lastTime = currentTime;

		handleEvents();
		update(deltaTime);
		render();

		uint64_t frameTime = SDL_GetTicks() - currentTime;
		if (frameTime < m_iFrameDelay)
			SDL_Delay(uint32_t(m_iFrameDelay - frameTime));
	}

	// If run is complete, quit the app automatically
	quit();
}

void SDLApplication::initGlobalServices()
{
	// Register EventBus as global service
	ServiceLocator::registerService(new EventBus());

	// Register ResourceManager as global service
	ServiceLocator::registerService(new ResourceManager("assets/AssetsIDs.json", m_pRenderer, m_sfxMixer, m_musicMixer));

	ServiceLocator::registerService(new AudioManager(m_sfxMixer, m_musicMixer));

	// Register InputManager as global service
	auto const inputManager = new InputManager();
	ServiceLocator::registerService(inputManager);
	m_inputManager = inputManager;

	// Register TweenManager as global service
	ServiceLocator::registerService(new TweenManager());

	// Register AppFacadeService as global service
	ServiceLocator::registerService(new AppFacadeService(this));

	// Register SceneManager as global service
	m_sceneManager = new SceneManager();
	ServiceLocator::registerService(m_sceneManager);

}

void SDLApplication::handleEvents() {
	m_inputManager->handleInputs();
}

void SDLApplication::update(float deltaTime)
{
	m_sceneManager->update(deltaTime);

	TweenManager* t = ServiceLocator::tryGet<TweenManager>();
	if (t) t->update();
}

void SDLApplication::render()
{
	// Clear screen
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer);

	// Render scenes
	m_sceneManager->render();
	
	//m_SceneManager->render();
	SDL_RenderPresent(m_pRenderer);
}

void SDLApplication::quit()
{
	ServiceLocator::unregister<SceneManager>();

	// Clear Event Bus events
	EventBus* eb = ServiceLocator::tryGet<EventBus>();
	if (eb != nullptr) {
		eb->clearAllEventListeners();
	}
	
	// clear all services in appropiate order
	ServiceLocator::unregister<AudioManager>();
	ServiceLocator::unregister<TweenManager>();
	ServiceLocator::unregister<InputManager>();
	ServiceLocator::unregister<ResourceManager>();
	ServiceLocator::unregister<EventBus>();
	ServiceLocator::unregister<AppFacadeService>();
	ServiceLocator::clear();

	// Destroy renderer
	if (m_pRenderer) {
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}

	// Destroy window
	if (m_pWindow) {
		SDL_DestroyWindow(m_pWindow);
		m_pWindow = nullptr;
	}

	// Destroy mixers
	if (m_sfxMixer) {
		MIX_DestroyMixer(m_sfxMixer);
		m_sfxMixer = nullptr;
	}

	if (m_musicMixer) {
		MIX_DestroyMixer(m_musicMixer);
		m_musicMixer = nullptr;
	}

	MIX_Quit();

	// Quit ttf
	TTF_Quit();

	// Quit SDL
	SDL_Quit();
}