#pragma once
#include "SDL3/SDL.h"
#include "../Math/Vec2.h"
#include "../Math/Mat3.h"
#include <vector>
#include <string>
#include "../Tools/ServiceLocator.h"
#include "../Resources/ResourceManager.h"

#include "../Resources/Texture.h"
#include "../Tools/ObjectPool.h"

struct Particle
{
    Particle(Vec2f position, Vec2f velocity, Vec2f acceleration,
        float rotation, float angularVelocity,
        float scale,
        float maxLifetime,
        SDL_Color color)
        : position(position), velocity(velocity), acceleration(acceleration),
        rotation(rotation), angularVelocity(angularVelocity),
        scale(scale), lifetime(0), maxLifetime(maxLifetime),
        color(color), active(true)
    {}
    Vec2f position, velocity, acceleration;
    float rotation, angularVelocity;
    float scale;
    float lifetime, maxLifetime;
    SDL_Color color;
    bool active;
};

struct ParticleEmitter
{
    Texture* m_particleTexture;
    std::string m_particleTextureID;

    ObjectPool<Particle> m_particlePool;
    std::vector<Particle*> m_activeParticles;

    Vec2f m_spawnArea;
    Vec2f m_minVelocity, m_maxVelocity;
    Vec2f m_minAcceleration, m_maxAcceleration;
    float m_minAngularVel, m_maxAngularVel;
    float m_minRotation, m_maxRotation;
    float m_minScale, m_maxScale;
    float m_minLifetime, m_maxLifetime;
    SDL_Color m_particleColor;

    bool m_isEmitting;
    float m_emissionRate;
    std::size_t m_emissionAmount;
    float m_elapsed;

    ParticleEmitter(std::size_t maxParticles, const std::string& particleId, float emissionRate = 0.5f, bool startEmitting = false, std::size_t emissionAmount = 1) : 
        m_particlePool(maxParticles),
        m_spawnArea({0,0}),
        m_minVelocity(0,0), m_maxVelocity(0,0),
        m_minAcceleration(0,0), m_maxAcceleration(0,0),
        m_minAngularVel(0), m_maxAngularVel(0),
        m_minRotation(0), m_maxRotation(0),
        m_minScale(1), m_maxScale(1),
        m_minLifetime(1), m_maxLifetime(1),
        m_isEmitting(startEmitting), m_emissionRate(emissionRate), m_emissionAmount(emissionAmount), m_elapsed(0)
    {
        setParticleTexture(particleId);
    }

    ParticleEmitter(ParticleEmitter&& other) noexcept :
        m_particleTexture(other.m_particleTexture), m_particleTextureID(other.m_particleTextureID),
        m_particlePool(std::move(other.m_particlePool)), m_activeParticles(std::move(other.m_activeParticles)),
        m_spawnArea(other.m_spawnArea),
        m_minVelocity(other.m_minVelocity), m_maxVelocity(other.m_maxVelocity),
        m_minAcceleration(other.m_minAcceleration), m_maxAcceleration(other.m_maxAcceleration),
        m_minAngularVel(other.m_minAngularVel), m_maxAngularVel(other.m_maxAngularVel),
        m_minRotation(other.m_minRotation), m_maxRotation(other.m_maxRotation),
        m_minScale(other.m_minScale), m_maxScale(other.m_maxScale),
        m_minLifetime(other.m_minLifetime), m_maxLifetime(other.m_maxLifetime),
        m_particleColor(other.m_particleColor),
        m_isEmitting(other.m_isEmitting), m_emissionRate(other.m_emissionRate), m_emissionAmount(other.m_emissionAmount), m_elapsed(other.m_elapsed)
    {
        other.m_particleTexture = nullptr;
        other.m_particleTextureID.clear();
    }

    ~ParticleEmitter()
    {
        if(!m_particleTextureID.empty())
            ServiceLocator::get<ResourceManager>().ReleaseResource<Texture>(m_particleTextureID);
    }
    
    ParticleEmitter(const ParticleEmitter&) = delete;
    ParticleEmitter& operator=(ParticleEmitter&& other) noexcept
    {
        if (&other == this) return *this;

        if (!m_particleTextureID.empty())
            ServiceLocator::get<ResourceManager>().ReleaseResource<Texture>(m_particleTextureID);

        m_particleTexture = other.m_particleTexture;
        m_particleTextureID = other.m_particleTextureID;

        m_particlePool = std::move(other.m_particlePool);
        m_activeParticles = std::move(other.m_activeParticles);
        m_spawnArea = other.m_spawnArea;
        m_minVelocity = other.m_minVelocity; m_maxVelocity = other.m_maxVelocity;
            m_minAcceleration = other.m_minAcceleration; m_maxAcceleration = other.m_maxAcceleration;
        m_minAngularVel = other.m_minAngularVel; m_maxAngularVel = other.m_maxAngularVel;
        m_minRotation = other.m_minRotation; m_maxRotation = other.m_maxRotation;
        m_minScale = other.m_minScale; m_maxScale = other.m_maxScale;
        m_minLifetime = other.m_minLifetime; m_maxLifetime = other.m_maxLifetime;
        m_particleColor = other.m_particleColor;
        m_isEmitting = other.m_isEmitting; m_emissionRate = other.m_emissionRate; m_emissionAmount = other.m_emissionAmount; m_elapsed = other.m_elapsed;

        other.m_particleTexture = nullptr;
        other.m_particleTextureID.clear();

        return *this;
    }

    inline void setSpawnArea(Vec2f area) { m_spawnArea = area/2; } // To avoid diving when emitting particles

    inline void setParticleVelocity(Vec2f vel) { setParticleVelocity(vel, vel); }
    inline void setParticleVelocity(Vec2f min, Vec2f max) { m_minVelocity = min; m_maxVelocity = max; }

    inline void setParticleAcceleration(Vec2f acc) { setParticleAcceleration(acc, acc); }
    inline void setParticleAcceleration(Vec2f min, Vec2f max) { m_minAcceleration = min; m_maxAcceleration = max; }

    inline void setParticleAngularVel(float angularVel) { setParticleAngularVel(angularVel, angularVel); }
    inline void setParticleAngularVel(float min, float max) { m_minAngularVel = min; m_maxAngularVel = max; }

    inline void setParticleRotation(float rot) { setParticleRotation(rot, rot); }
    inline void setParticleRotation(float min, float max) { m_minRotation = min; m_maxRotation = max; }

    inline void setParticleScale(float scale) { setParticleScale(scale, scale); }
    inline void setParticleScale(float min, float max) { m_minScale = min; m_maxScale = max; }

    inline void setParticleLifetime(float lifetime) { setParticleLifetime(lifetime, lifetime); }
    inline void setParticleLifetime(float min, float max) { m_minLifetime = min; m_maxLifetime = max; }

    inline void setParticleColor(SDL_Color color) { m_particleColor = color; }

    inline void setParticleTexture(const std::string& id)
    {
        if (id == m_particleTextureID) return;
        
        if(!m_particleTextureID.empty())
            ServiceLocator::get<ResourceManager>().ReleaseResource<Texture>(m_particleTextureID);

        m_particleTextureID = id;
        m_particleTexture = ServiceLocator::get<ResourceManager>().GetResource<Texture>(m_particleTextureID);
    }

    inline void setEmissionAmount(std::size_t amount) { m_emissionAmount = amount; }
    inline void setEmissionRate(float emissionRate) { m_emissionRate = emissionRate; }
    inline void startEmitting() { m_isEmitting = true; }
    inline void stopEmitting() { m_isEmitting = false; }
    inline void setEmitting(bool emit) { m_isEmitting = emit; }
    inline bool isEmitting() { return m_isEmitting; }

    void emit(std::size_t amount);
};

