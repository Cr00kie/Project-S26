#include "SpriteAnimatorHandle.h"

void SpriteAnimatorHandle::play() { get().play(); }

void SpriteAnimatorHandle::stop() { get().stop(); }

bool SpriteAnimatorHandle::isPlaying() const { return get().isPlaying(); }

void SpriteAnimatorHandle::setPlaying(bool playing) { get().setPlaying(playing); }

void SpriteAnimatorHandle::setAnimation(const std::string& id) { get().setAnimation(id); }
