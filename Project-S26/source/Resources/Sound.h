#pragma once

#include "BasicResource.h"
#include <utility>
#include <SDL3_mixer/SDL_mixer.h>
#include <stdexcept>

// A resource that represents a sound loaded from a file path
class Sound :
	public BasicResource
{
private:
	// The SDL_mixer audio that represents the sound resource
    MIX_Audio* m_pAudio = { nullptr };
    
	// Release the sound resource by destroying the SDL_mixer audio and setting the pointer to nullptr
    void release() noexcept {
        if (m_pAudio) {
            MIX_DestroyAudio(m_pAudio);
            m_pAudio = nullptr;
        }
    }

public:
    // Creates a Sound resource which plays a SFX
    // Arguments:
    //		- mixer: the mixer object
    //      - audio: the SFX audio to be played
    Sound(MIX_Audio* audio);
    ~Sound() { release(); };

	// Delete copy constructor and assignment operator to prevent copying of sounds
    Sound(const Sound&) = delete;
    Sound& operator=(const Sound&) = delete;

    Sound(Sound&& other) noexcept {
        m_pAudio = std::exchange(other.m_pAudio, nullptr);
    }

	// Move assignment operator to transfer ownership of the sound resource from another Sound object,
    Sound& operator=(Sound&& other) noexcept {
        if (this != &other) {
            release();
            m_pAudio = std::exchange(other.m_pAudio, nullptr);
        }
        return *this;
    }

    inline MIX_Audio* getMixAudio() const
    {
        return m_pAudio;
    }
};

