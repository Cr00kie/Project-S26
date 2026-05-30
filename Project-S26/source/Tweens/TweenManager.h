#pragma once
#include <vector>

class Tween;
//Manages the tweens of the application,
//updating them and deleting them when they are finished
class TweenManager {
	friend Tween;
private:
	std::vector<Tween*> m_Tweens;

	//Adds a tween to the manager,
	void addTween(Tween*);
	//Deletes the finished tweens from the HEAP and from the vector.
	void cleanup();
public:
	//Delete copy constructor and assignment operator to avoid copying the manager
	TweenManager& operator=(const TweenManager&) = delete;
	TweenManager(const TweenManager&) = delete;

	TweenManager() = default;
	~TweenManager();

	//Updates all tweens, deleting the finished ones
	void update();
	//Finish and erase all current tweens.
	void resetManager();
};