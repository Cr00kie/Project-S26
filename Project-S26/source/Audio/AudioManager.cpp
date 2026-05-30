#include "AudioManager.h"

AudioManager::AudioManager(MIX_Mixer* sfx_mixer, MIX_Mixer* music_mixer) :
	m_pSFXMixer(sfx_mixer),
	m_pMusicMixer(music_mixer),
	m_sfxVolume(1.0f),
	m_musicVolume(1.0f)
{
}

AudioManager::~AudioManager()
{
	for (Track& track : m_SFXTracks)
	{
		//MIX_SetTrackStoppedCallback(track.mixTrack, 0, nullptr);
		MIX_DestroyTrack(track.mixTrack);
	}

	MIX_DestroyTrack(m_MusicTrack.mixTrack);
}

void AudioManager::playAudio(std::string audioKey)
{
	Sound* audio = ServiceLocator::tryGet<ResourceManager>()->GetResource<Sound>(audioKey);
	if (!audio) return;

	// Buscamos una tack libre
	Track& track = getFreeTrack();
	track.used = true;
	track.audioKey = audioKey;

	// Play audio
	MIX_SetTrackStoppedCallback(track.mixTrack, OnTrackStopped, this);
	MIX_SetTrackAudio(track.mixTrack, audio->getMixAudio());
	MIX_PlayTrack(track.mixTrack, 0);
}

void AudioManager::playMusic(std::string musicID, int loops, Uint64 fadeIn) {
	if (musicID == m_currentMusicID) return; // Si la música que queremos reproducir ya se está reproduciendo, no hacemos nada

	// If the music track is nullptr, we create it.
	if (!m_MusicTrack.mixTrack) {
		m_MusicTrack.mixTrack = MIX_CreateTrack(m_pMusicMixer);
	}
	// Si ya se estaba usando se para antes de poner el nuevo audio.
	if (m_MusicTrack.used) {
		m_waitingMusic = { true, musicID, loops, fadeIn };
		stopMusic();
	}
	else {
		startMusic(musicID, loops, fadeIn);
	}
}

void AudioManager::stopMusic(Uint64 fadeOut)
{
	if (!m_MusicTrack.mixTrack || !m_MusicTrack.used) return;

	// Stop music
	Sint64 fadeOutFrames = MIX_TrackMSToFrames(m_MusicTrack.mixTrack, fadeOut);
	MIX_StopTrack(m_MusicTrack.mixTrack, fadeOutFrames);
}

void SDLCALL AudioManager::OnTrackStopped(void* userdata, MIX_Track* track)
{
	// Cast userdata back to the instance
	auto* self = static_cast<AudioManager*>(userdata); // No se como hacerlo sin este casteo, a no ser que lo haga todo en puro C
	self->HandleTrackStopped(track);
}

void AudioManager::HandleTrackStopped(MIX_Track* mixTrack)
{
	// Ponemos la track como libre
	Track* track = findTrack(mixTrack);
	if (track)
	{
		track->used = false;
		ServiceLocator::tryGet<ResourceManager>()->ReleaseResource<Sound>(track->audioKey);
	}
	else if (m_MusicTrack.mixTrack == mixTrack) {
		m_MusicTrack.used = false;
		ServiceLocator::tryGet<ResourceManager>()->ReleaseResource<Sound>(m_MusicTrack.audioKey);
		// Free music track
		MIX_SetTrackAudio(m_MusicTrack.mixTrack, nullptr);

		// Si había una música esperando a ser reproducida, la reproducimos ahora que la track está libre
		if (m_waitingMusic.waiting) {
			startMusic(m_waitingMusic.musicID, m_waitingMusic.loops, m_waitingMusic.fadeIn);
			m_waitingMusic.waiting = false;
		}
	}

	// Miramos si sobran tracks, y borramos si sobran 
	shrinkPoolIfNeeded();
}

void AudioManager::setSFXVolume(float v)
{
	m_sfxVolume = std::clamp(v, 0.0f, 1.0f);
	MIX_SetMixerGain(m_pSFXMixer, m_sfxVolume);
}

void AudioManager::setMusicVolume(float v)
{
	m_musicVolume = std::clamp(v, 0.0f, 1.0f);
	MIX_SetMixerGain(m_pMusicMixer, m_musicVolume);
}

void AudioManager::startMusic(std::string musicID, int loops, Uint64 fadeIn) {
	Sound* music = ServiceLocator::tryGet<ResourceManager>()->GetResource<Sound>(musicID);
	if (!music) return;

	m_MusicTrack.used = true;
	m_MusicTrack.audioKey = musicID;
	m_currentMusicID = musicID;
	//Play music
	MIX_SetTrackStoppedCallback(m_MusicTrack.mixTrack, OnTrackStopped, this);
	MIX_SetTrackAudio(m_MusicTrack.mixTrack, music->getMixAudio());
	//Properties for the fade in, 
	//we set the fade in time in milliseconds using the MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER property
	SDL_PropertiesID props = SDL_CreateProperties();
	SDL_SetNumberProperty(props, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, fadeIn);
	MIX_PlayTrack(m_MusicTrack.mixTrack, props);
	SDL_DestroyProperties(props);
	MIX_SetTrackLoops(m_MusicTrack.mixTrack, loops);
}

void AudioManager::shrinkPoolIfNeeded()
{
	// Quitamos tracks solo si hay mas de 5 (por ejemplo ns) y sobran tracks libres
	while (m_SFXTracks.size() > 5 && !m_SFXTracks.back().used)
	{
		MIX_DestroyTrack(m_SFXTracks.back().mixTrack);
		m_SFXTracks.pop_back();
	}
}
