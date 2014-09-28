#include "FlyCamera.h"

using namespace krono;
using namespace kge;

FlyCamera::FlyCamera(kge::GameObject& parentGameObject) :
	Behavior(parentGameObject),
	mMoveSpeed(1.0f),
	mRotationRate(Constant<float>::PI * 100.0f)
{

}


FlyCamera::~FlyCamera(void)
{

}

void FlyCamera::Update(float deltaTime)
{
	const InputState& inputState = GetInputState();

	if (inputState.GetMouseButton(MouseButton::LeftButton))
	{
		Vector2f angleMovement = (Vector2f)inputState.GetMouseMove() * (mRotationRate * deltaTime / inputState.GetScreenSize().y);
		Quaternionf newRotation = 
			Quaternionf(Vector3f(0.0f, 1.0f, 0.0f), Radiansf(angleMovement.x)) *
			mGameObject.GetTransform()->GetLocalOrientation() * 
			Quaternionf(Vector3f(1.0f, 0.0f, 0.0f), Radiansf(angleMovement.y));
		
		mGameObject.GetTransform()->SetLocalOrientation(newRotation);
	}

	Vector3f translation;

	if (inputState.GetKey(KeyCode::W))
	{
		translation += Vector3f(0.0f, 0.0f, 1.0f);
	}

	if (inputState.GetKey(KeyCode::S))
	{
		translation -= Vector3f(0.0f, 0.0f, 1.0f);
	}

	if (inputState.GetKey(KeyCode::D))
	{
		translation += Vector3f(1.0f, 0.0f, 0.0f);
	}

	if (inputState.GetKey(KeyCode::A))
	{
		translation -= Vector3f(1.0f, 0.0f, 0.0f);
	}

	if (inputState.GetKey(KeyCode::R))
	{
		translation += Vector3f(0.0f, 1.0f, 0.0f);
	}

	if (inputState.GetKey(KeyCode::F))
	{
		translation -= Vector3f(0.0f, 1.0f, 0.0f);
	}

	translation *= mMoveSpeed * deltaTime;

	mGameObject.GetTransform()->SetLocalPosition(
		mGameObject.GetTransform()->GetLocalPosition() +
		mGameObject.GetTransform()->GetLocalOrientation().Rotate(translation));
}