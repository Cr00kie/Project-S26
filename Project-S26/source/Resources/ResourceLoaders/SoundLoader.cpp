#include "SoundLoader.h"

SoundLoader::SoundLoader(MIX_Mixer* mixer) {
	m_pMixer = mixer;

	if (!m_pMixer)
		throw std::runtime_error("Mixer not propertly loaded to audio loader");
}

std::type_index SoundLoader::ResourceType() const {
	return typeid(Sound);
}

BasicResource* SoundLoader::Load(const std::string& path) {
	const char* char_path = path.c_str();
	MIX_Audio* audio = MIX_LoadAudio(m_pMixer, char_path, false);
	return new Sound(audio);
}
