#include "Sound.h"

Sound::Sound(MIX_Audio* audio) {
	m_pAudio = audio;

	if (!m_pAudio)
		throw std::runtime_error("Audio not propertly loaded");
}