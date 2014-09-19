#pragma once

#include <BasicGameEngine.h>
#include <Krono.h>

class SpinBehavior : public kge::Behavior
{
public:
	SpinBehavior(kge::GameObject& parentGameObject);
	~SpinBehavior(void);

	virtual void Update(float deltaTime);
private:
	krono::Vector3f mAxis;
	float mRotationRate;
};

