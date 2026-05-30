#pragma once
#include <list>
#include <functional>

class TweenManager;

//interpolates a value between a start and an end value in a given time,
//calling a function with the interpolated value every frame
class Tween {
	//Allows TweenManager to use private functions from Tween class
	friend TweenManager;
public:
	//Enum of the different easing functions that can be used in the tween
	enum ease {
		LINEAR,
		EXPO_IN,
		EXPO_OUT,
		EXPO_IN_OUT,
		SINE_IN,
		SINE_OUT,
		SINE_IN_OUT,
		QUAD_IN,
		QUAD_OUT,
		QUAD_IN_OUT
	};
private:
	//Pointer to the tween manager,
	//used to add the tween to it when it is created
	TweenManager* m_Manager;
	//Target FPS of the application,
	//used to calculate the time between frames
	int m_iFPS;

	//Function to call every frame with the interpolated value
	std::function<void(float)> m_lmb;
	//Function to call when the tween is finished
	std::function<void()> m_onFinish;

	//Time tracking
	//Elapsed time since the start of the tween in miliseconds
	float m_fElapsedTime;
	//Duration of the tween in miliseconds
	float m_fDuration;

	//Start and end values of the tween
	float m_fStartValue;
	float m_fEndValue;

	//Delay before the tween starts in miliseconds
	float m_fDelay;

	//How many times its repeats
	int m_iRepeat;

	//Easing function to use in the tween
	ease m_ease;

	//Flag to indicate if the tween is finished.
	bool m_bFinished;

	//Direction of the tween, 1 for forward, -1 for backward
	float m_fDirection;

	//Constructor, initializes the tween and adds it to the tween manager
	// Arguments:
	//		- startValue: the initial value of the tween
	//		- destinationValue: the end value of the tween
	//		- duration: the duration of the tween to interpolate from start to end
	//      - callback: recieves the interpolated value and manages it
	//      - repeat: how many times repeats the tween, -1 = infinite
	//		- delay: time to pass before starting to interpolate the values
	//		- ease: type of interpolation to apply to the tween
	Tween(float startValue, float destination, float duration, std::function<void(float)> callback, int repeat = 0, float delay = 0, Tween::ease ease = LINEAR);

	//Updates the tween,
	//calculating the new interpolated value and calling the function with it.
	void update();
public:
	//makes a new tween returning a pointer to it
	//Constructor, initializes the tween and adds it to the tween manager
	// Arguments:
	//		- startValue: the initial value of the tween
	//		- destinationValue: the end value of the tween
	//		- duration: the duration of the tween to interpolate from start to end
	//      - callback: recieves the interpolated value and manages it
	//		- delay: time to pass before starting to interpolate the values
	//		- ease: type of interpolation to apply to the tween
	static Tween* makeTween(float startValue, float destination, float duration, std::function<void(float)> callback, int repeat = 0, float delay = 0, Tween::ease ease = LINEAR) {
		return new Tween(startValue, destination, duration, callback, repeat, delay, ease);
	}

	~Tween() = default;

	//Finishes the tween,
	//calling the onComplete function and marking it as finished
	void finish();

	//Returns true if the tween is finished, false otherwise
	bool isFinished() const { return m_bFinished; }

	//Sets the function to call when the tween is finished
	void onComplete(std::function<void()> callback) { m_onFinish = callback; }

	//changes the remaining time of the tween, keeping the same progress
	void setRemainingTime(float remainingTime);
};