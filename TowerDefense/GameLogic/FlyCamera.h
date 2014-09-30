#pragma once

#include <BasicGameEngine.h>
#include <Krono.h>

class FlyCamera : public kge::Behavior
{
public:
	typedef std::shared_ptr<FlyCamera> Ptr;
	typedef std::unique_ptr<FlyCamera> Ref;

	FlyCamera(kge::GameObject& parentGameObject);
	~FlyCamera(void);
	
	virtual void Update(float deltaTime);
private:
	float mMoveSpeed;
	float mRotationRate;
};

