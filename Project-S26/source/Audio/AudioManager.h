#pragma once
#include "SDL3_mixer/SDL_mixer.h"

#include <string>
#include <vector>

#include "../Resources/ResourceManager.h"
#include "../Tools/ServiceLocator.h"
#include "../Resources/Sound.h"

#include "../Lua/LuaBindingTools.h"

class AudioManager
{
	LUA_EXPOSE_TYPE(AudioManager, AudioManager, NO_CONSTRUCTOR, NO_BASE)
private:

	// A struct that represents a track in the audio manager, 
	// it holds the MIX_Track pointer, the audio key and a boolean to indicate if the track is currently being used
	struct Track
	{
		MIX_Track* mixTrack = nullptr;
		std::string audioKey;
		bool used = false;
	};
	// A struct that represents a music track that is waiting to be played,
	// it holds the music id, the number of loops and the fade in time
	struct waitingMusic {
		bool waiting = false;
		std::string musicID;
		int loops;
		Uint64 fadeIn;
	};
	// SDL_mixer mixers for music and sfx, they are used to play the audio resources
	MIX_Mixer* m_pSFXMixer;
	MIX_Mixer* m_pMusicMixer;

	// Volume values for music and sfx
	float m_sfxVolume;
	float m_musicVolume;

	// A vector that holds the tracks for playing sfx, it is used as a pool of tracks to avoid creating 
	// and destroying tracks every time we want to play a sound
	std::vector<Track> m_SFXTracks;
	Track m_MusicTrack;
	waitingMusic m_waitingMusic;
	std::string m_currentMusicID = "";
public:
	// Creates an AudioManager to manage the audio of the game
	//	- sfx_mixer: the SDL_mixer mixer used to play sound effects
	//	- music_mixer: the SDL_mixer mixer used to play music
	AudioManager(MIX_Mixer* sfx_mixer, MIX_Mixer* music_mixer);

	// Destructor of the AudioManager, it destroys all the tracks in the pool to free the resources
	~AudioManager();

	// Plays an audio resource with the given key, it will look for the audio resource in the ResourceManager and play it using a free track from
	// the pool, if there are no free tracks it will create a new one and add it to the pool
	//  - audioKey: the key of the audio resource to play
	void playAudio(std::string audioKey);
	LUA_EXPOSE_MEMBER(AudioManager, playAudio, play_sound)
	// Plays a music track with the given id,
	// it will look for the music resource in the ResourceManager and play it using the music track, if the music track is already being used it will stop it before playing the new one.
	// loops parameter specifies how many times to loop the music track after it finishes playing once,
	// it can be 0 for no looping, -1 for infinite looping, or any positive integer for a specific number of loops
	// fadeIn parameter specifies the time in milliseconds to fade in the music, it can be 0 for no fade in
	void playMusic(std::string musicID, int loops = -1, Uint64 fadeIn = 1000);
	// Stops the music track if it is currently being used,
	// it will fade out the music over the given time in milliseconds
	void stopMusic(Uint64 fadeOut = 1000);

	// Callback function that is called when a track stops playing, it is used to mark the track as free and 
	// release the audio resource from the ResourceManager
	//  - userdata: a pointer to the AudioManager instance
	//  - track: the MIX_Track pointer of the track that stopped playing
	static void SDLCALL OnTrackStopped(void* userdata, MIX_Track* track);

	// Handles the track stopped event, it is called by the OnTrackStopped callback 
	// function to mark the track as free and release the audio resource from the ResourceManager
	//  - mixTrack: the MIX_Track pointer of the track that stopped playing
	void HandleTrackStopped(MIX_Track* mixTrack);
	
	// Function to set the volume of the music, it will clamp the volume value between 0 and 1 and set the gain of the corresponding mixer
	//  - v: the volume value to set, it should be between 0 and 1
	void setSFXVolume(float v);
	// Function to set the volume of the sfx, it will clamp the volume value between 0 and 1 and set the gain of the corresponding mixer
	//  - v: the volume value to set, it should be between 0 and 1
	void setMusicVolume(float v);


	//Getters for the volume values of music and sfx
	// 
	// Returns the current volume value for sfx, it is a float between 0 and 1
	float getSFXVolume() const { return m_sfxVolume; }
	// Returns the current volume value for music, it is a float between 0 and 1
	float getMusicVolume() const { return m_musicVolume; }

private:
	void startMusic(std::string musicID, int loops, Uint64 fadeIn);
	// Searches for a free track in the pool, if it finds one it returns it, 
	// if it doesn't find one it creates a new track, adds it to the pool and returns it
	Track& getFreeTrack()
	{
		for (Track& track : m_SFXTracks)
		{
			if (!track.used)
				return track;
		}

		// No hay tracks libres, creamos una
		Track& newTrack = m_SFXTracks.emplace_back();
		newTrack.mixTrack = MIX_CreateTrack(m_pSFXMixer);
		return newTrack;
	}

	// Returns a pointer to the track in the pool that has the given MIX_Track pointer, if it doesn't find one it returns nullptr
	Track* findTrack(MIX_Track* mixTrack)
	{
		for (Track& track : m_SFXTracks)
		{
			if (track.mixTrack == mixTrack)
				return &track;
		}
		return nullptr;
	}

	// Checks if there are too many tracks in the pool that are not being used, and if so it destroys them to free resources, 
	void shrinkPoolIfNeeded();
};

