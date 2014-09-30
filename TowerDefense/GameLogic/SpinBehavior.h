#pragma once

#include <BasicGameEngine.h>
#include <Krono.h>

class SpinBehavior : public kge::Behavior
{
public:
	typedef std::shared_ptr<SpinBehavior> Ptr;
	typedef std::unique_ptr<SpinBehavior> Ref;

	SpinBehavior(kge::GameObject& parentGameObject);
	~SpinBehavior(void);

	void SetAxis(const krono::Vector3f& axis);
	void SetRotationRate(krono::Degreesf radsPerSec);

	virtual void Update(float deltaTime);
private:
	krono::Vector3f mAxis;
	float mRotationRate;
};

