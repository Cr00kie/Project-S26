#pragma once

#include "../LuaBindingTools.h"
#include "BaseComponentHandle.h"
#include "../../Components/ParticleEmitter.h"

class ParticleEmitterHandle : public BaseComponentHandle<ParticleEmitter>
{
	LUA_EXPOSE_TYPE(ParticleEmitterHandle, ParticleEmitterHandle, NO_CONSTRUCTOR, NO_BASE)
public:
	ParticleEmitterHandle(Registry& reg, Entity e) : BaseComponentHandle(reg, e) {}

    void setSpawnArea(float x, float y);
    LUA_EXPOSE_MEMBER(ParticleEmitterHandle, setSpawnArea, set_spawn_area)

    void setParticleVelocity(float x0, float y0, float x1, float y1);
    LUA_EXPOSE_MEMBER(ParticleEmitterHandle, setParticleVelocity, set_particle_velocity)

    void setParticleAcceleration(float x0, float y0, float x1, float y1);
    LUA_EXPOSE_MEMBER(ParticleEmitterHandle, setParticleAcceleration, set_particle_acceleration)

    void setParticleAngularVel(float min, float max);
    LUA_EXPOSE_MEMBER(ParticleEmitterHandle, setParticleAngularVel, set_particle_angular_vel)

    void setParticleRotation(float min, float max);
    LUA_EXPOSE_MEMBER(ParticleEmitterHandle, setParticleRotation, set_particle_rotation)

    void setParticleScale(float min, float max);
    LUA_EXPOSE_MEMBER(ParticleEmitterHandle, setParticleScale, set_particle_scale)

    void setParticleLifetime(float min, float max);
    LUA_EXPOSE_MEMBER(ParticleEmitterHandle, setParticleLifetime, set_particle_lifetime)

    void setParticleColor(int r, int g, int b, int a);
    LUA_EXPOSE_MEMBER(ParticleEmitterHandle, setParticleColor, set_particle_color)
    
    void setParticleTexture(const std::string& id);
    LUA_EXPOSE_MEMBER(ParticleEmitterHandle, setParticleTexture, set_particle_texture)

    void setEmissionAmount(std::size_t amount);
    void setEmissionRate(float emissionRate);
    void startEmitting();
    void stopEmitting();
    void setEmitting(bool emit);
    bool isEmitting();
    LUA_EXPOSE_MEMBER(ParticleEmitterHandle, setEmissionAmount, set_emission_amount)
    LUA_EXPOSE_MEMBER(ParticleEmitterHandle, setEmissionRate, set_emission_rate)
    LUA_EXPOSE_MEMBER(ParticleEmitterHandle, startEmitting, start_emitting)
    LUA_EXPOSE_MEMBER(ParticleEmitterHandle, stopEmitting,stop_emitting)
    LUA_EXPOSE_MEMBER(ParticleEmitterHandle, setEmitting, set_emitting)
    LUA_EXPOSE_MEMBER(ParticleEmitterHandle, isEmitting, is_emitting)

    void emit(int amount) { get().emit(amount); }
    LUA_EXPOSE_MEMBER(ParticleEmitterHandle, emit, emit)
};
LUA_EXPOSE_COMPONENT_GETTER(ParticleEmitterHandle, get_particle_emitter)