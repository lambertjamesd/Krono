
#include "Lens.h"
#include <iostream>
namespace kge
{

using namespace krono;

Lens::Lens(float nearPlane, float farPlane) :
	mNear(nearPlane),
	mFar(farPlane)
{

}

Lens::~Lens()
{

}


PerspectiveLens::PerspectiveLens(float nearPlane, float farPlane, const Radiansf& verticalFov) :
	Lens(nearPlane, farPlane),
	mVerticalFov(verticalFov)
{

}

Matrix4f PerspectiveLens::GetProjectionMatrix() const
{
	Matrix4f result(Matrix4f::Identity());

	float tanFov = Math<float>::Tan(mVerticalFov * 0.5f);

	float cotFov = 1.0f / tanFov;
	float zScale = mFar / (mFar - mNear);

	result.At(0, 0) = cotFov;
	result.At(1, 1) = cotFov;
	result.At(2, 2) = zScale;

	result.At(2, 3) = -mNear * zScale;
	result.At(3, 2) = 1.0f;

	result.At(3, 3) = 0.0f;

	return result;
}


OrthographicLens::OrthographicLens(float nearPlane, float farPlane, float height) :
	Lens(nearPlane, farPlane),
	mHeight(height)
{

}

Matrix4f OrthographicLens::GetProjectionMatrix() const
{
	Matrix4f result(Matrix4f::Identity());

	result.At(0, 0) = 2.0f / mHeight;
	result.At(1, 1) = 2.0f / mHeight;
	result.At(2, 2) = 1.0f / (mFar - mNear);

	result.At(2, 3) = mNear / (mNear - mFar);

	return result;
}

}
