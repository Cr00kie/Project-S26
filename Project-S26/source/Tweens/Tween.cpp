#include <string>
#include <algorithm>
#include <numbers>

#include "Tween.h"
#include "TweenManager.h"
#include "../Events/EventBus.h"
#include "../AppFacadeService.h"
#include "../Tools/ServiceLocator.h"
#include "../SDLApplication.h"

Tween::Tween(float startValue, float destination, float duration, std::function<void(float)> callback, int repeat, float delay, ease ease)
	:
	m_Manager(ServiceLocator::tryGet<TweenManager>()),
	m_lmb(callback),
	m_fElapsedTime(0.0f),
	m_fDuration(duration),
	m_fStartValue(startValue),
	m_fEndValue(destination),
	m_iRepeat(repeat),
	m_fDelay(delay),
	m_ease(ease),
	m_bFinished(false),
	m_fDirection(1.0f) {

	// Get app FPS
	AppFacadeService* app = ServiceLocator::tryGet<AppFacadeService>();
	if (app) m_iFPS = app->getFPS();
	else m_iFPS = 60;

	// Automatically add self to tween manager
	if (m_Manager) {
		m_Manager->addTween(this);
	}
}

void
Tween::update() {
	if (m_bFinished) return;

	if (m_fDelay > 0) {
		// Update delay counter
		m_fDelay -= 1000.0f / m_iFPS;
		if (m_fDelay < 0.0f) m_fDelay = 0.0f;

		return;
	}

	// Calculate time per frame and completion percent
	m_fElapsedTime += (1000.f / m_iFPS) * m_fDirection;
	// Clamp elapsed time to max & min duration
	m_fElapsedTime = std::clamp(m_fElapsedTime, 0.0f, m_fDuration);

	float t = m_fDuration != 0 ? m_fElapsedTime / m_fDuration : 1.0f;

	// Interpolate values depending of type of interpolation
	switch (m_ease) {
	case (LINEAR):
		m_lmb(m_fStartValue + (m_fEndValue - m_fStartValue) * t);
		break;
	case (EXPO_IN):
		m_lmb(m_fStartValue + (m_fEndValue - m_fStartValue) * ((t == 0.0f) ? 0.0f : pow(2.f, 10.f * (t - 1.f))));
		break;
	case (EXPO_OUT):
		m_lmb(m_fStartValue + (m_fEndValue - m_fStartValue) * ((t == 1.0f) ? 1.0f : 1.f - pow(2.f, -10.f * t)));
		break;
	case(EXPO_IN_OUT):
		if (t == 0.0f) m_lmb(m_fStartValue + (m_fEndValue - m_fStartValue) * 0.0f);
		else if (t == 1.0f) m_lmb(m_fStartValue + (m_fEndValue - m_fStartValue) * 1.0f);
		else if (t < 0.5f) m_lmb(m_fStartValue + (m_fEndValue - m_fStartValue) * pow(2.0f, 20.0f * t - 10.0f) / 2.0f);
		else m_lmb(m_fStartValue + (m_fEndValue - m_fStartValue) * (2.0f - pow(2.0f, -20.0f * t + 10.0f)) / 2.0f);
		break;
	case(SINE_IN):
		m_lmb(m_fStartValue + (m_fEndValue - m_fStartValue) * (1.f - cos((t * std::numbers::pi_v<float>) / 2.f)));
		break;
	case(SINE_OUT):
		m_lmb(m_fStartValue + (m_fEndValue - m_fStartValue) * sin((t * std::numbers::pi_v<float>) / 2.f));
		break;
	case(SINE_IN_OUT):
		m_lmb(m_fStartValue + (m_fEndValue - m_fStartValue) * -(cos(std::numbers::pi_v<float> * t) - 1.f) / 2.f);
		break;
	case(QUAD_IN):
		m_lmb(m_fStartValue + (m_fEndValue - m_fStartValue) * (t * t));
		break;
	case (QUAD_OUT):
		m_lmb(m_fStartValue + (m_fEndValue - m_fStartValue) * (t * (2.f - t)));
		break;
	case (QUAD_IN_OUT):
		m_lmb(m_fStartValue + (m_fEndValue - m_fStartValue) * (t < 0.5f ? (2.f * t * t) : (-1.f + (4.f - 2.f * t) * t)));
		break;
	default: break;
	}

	// Check if interpolation ended
	if (t >= 1.0f) {
		if (m_iRepeat == 0) {
			m_lmb(m_fEndValue);
			finish();
		}
		else {
			m_fDirection *= -1.f;
		}
	}
	else if (t <= 0.0f && m_fDirection < 0.0f) {
		m_lmb(m_fStartValue);
		m_fDirection *= -1.f;
		m_iRepeat--;
	}
}

void Tween::finish() {
	if (m_onFinish) m_onFinish();
	m_bFinished = true;
}

void Tween::setRemainingTime(float remainingTime)
{
	float progress = m_fElapsedTime / m_fDuration;
	m_fDuration = remainingTime;  // a milisegundos
	m_fElapsedTime = progress * m_fDuration;
}
