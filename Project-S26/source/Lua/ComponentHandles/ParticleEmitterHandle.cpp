#include "ParticleEmitterHandle.h"

void ParticleEmitterHandle::setSpawnArea(float x, float y) { get().setSpawnArea({ x, y }); }

void ParticleEmitterHandle::setParticleVelocity(float x0, float y0, float x1, float y1) { get().setParticleVelocity({ x0, y0 }, { x1, y1 }); }

void ParticleEmitterHandle::setParticleAcceleration(float x0, float y0, float x1, float y1) { get().setParticleAcceleration({ x0, y0 }, { x1, y1 }); }

void ParticleEmitterHandle::setParticleAngularVel(float min, float max) { get().setParticleAngularVel(min, max); }

void ParticleEmitterHandle::setParticleRotation(float min, float max) { get().setParticleRotation(min, max); }

void ParticleEmitterHandle::setParticleScale(float min, float max) { get().setParticleScale(min, max); }

void ParticleEmitterHandle::setParticleLifetime(float min, float max) { get().setParticleLifetime(min, max); }

void ParticleEmitterHandle::setParticleColor(int r, int g, int b, int a) { get().setParticleColor({(unsigned char)r,(unsigned char)g,(unsigned char)b,(unsigned char)a}); }

void ParticleEmitterHandle::setParticleTexture(const std::string& id) { get().setParticleTexture(id); }

void ParticleEmitterHandle::setEmissionAmount(std::size_t amount) { get().setEmissionAmount(amount); }

void ParticleEmitterHandle::setEmissionRate(float emissionRate) { get().setEmissionRate(emissionRate); }

void ParticleEmitterHandle::startEmitting() { get().startEmitting(); }

void ParticleEmitterHandle::stopEmitting() { get().stopEmitting(); }

void ParticleEmitterHandle::setEmitting(bool emit) { get().setEmitting(emit); }

bool ParticleEmitterHandle::isEmitting() { return get().isEmitting(); }
