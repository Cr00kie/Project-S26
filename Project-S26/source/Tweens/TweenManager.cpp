#include "TweenManager.h"
#include "Tween.h"

TweenManager::~TweenManager() {
	resetManager();
}
void
TweenManager::update() {
	// Update tweens
	for (Tween* t : m_Tweens) t->update();
	cleanup();
}

void
TweenManager::addTween(Tween* t) {
	m_Tweens.push_back(t);
}

void TweenManager::cleanup() {
	auto it = m_Tweens.begin();

	while (it != m_Tweens.end()) {
		Tween* t = *it;

		if (t->m_bFinished) {
			delete t;
			it = m_Tweens.erase(it);
		}
		else ++it;
	}
}

void TweenManager::resetManager() {
	for (Tween* t : m_Tweens) {
		delete t;
	}
	m_Tweens.clear();
}
