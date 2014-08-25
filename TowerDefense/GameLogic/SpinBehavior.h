#pragma once

#include "GameObject/Behavior.h"
#include <Krono.h>

class SpinBehavior : public Behavior
{
public:
	SpinBehavior(GameObject& parentGameObject);
	~SpinBehavior(void);

	virtual void Update(float deltaTime);
private:
	krono::Vector3f mAxis;
	float mRotationRate;
};

