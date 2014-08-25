#include "SpinBehavior.h"
#include "GameObject/GameObject.h"

using namespace krono;

SpinBehavior::SpinBehavior(GameObject& parentGameObject) :
	Behavior(parentGameObject),

	mAxis(0.0f, 1.0f, 0.0f),
	mRotationRate(180.0f)
{

}


SpinBehavior::~SpinBehavior(void)
{

}

void SpinBehavior::Update(float deltaTime)
{
	Quaternionf rotationAmount(mAxis, Degreesf(deltaTime * mRotationRate));
	Quaternionf newOriention = mGameObject.GetTransform()->GetLocalOrientation() * rotationAmount;
	mGameObject.GetTransform()->SetLocalOrientation(newOriention);
}