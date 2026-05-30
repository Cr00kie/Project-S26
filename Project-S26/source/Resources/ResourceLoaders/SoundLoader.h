#pragma once

#include "ResourceLoader.h"
#include "../../Resources/Sound.h"

// A resource loader that loads sounds from file paths using SDL_mixer
class SoundLoader :
	public ResourceLoader
{
private:
	// The SDL_mixer mixer used to create the sound
	MIX_Mixer* m_pMixer = { nullptr };

public:
	// Creates a SoundLoader to load sounds from files
	// Arguments:
	//		- mixer: the mixer object being used
	SoundLoader(MIX_Mixer* mixer);
	~SoundLoader() = default;

	// Get the type of resource that this loader can load, which is Sound
	std::type_index ResourceType() const;
	// Load a sound from the given file path and return it as a BasicResource pointer
	BasicResource* Load(const std::string& path);
};

