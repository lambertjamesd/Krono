#pragma once

#include <BasicGameEngine.h>
#include <Krono.h>

class FlyCamera : public kge::Behavior
{
public:
	FlyCamera(kge::GameObject& parentGameObject);
	~FlyCamera(void);
	
	virtual void Update(float deltaTime);
private:
	float mMoveSpeed;
	float mRotationRate;
};

