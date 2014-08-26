#pragma once

#include <Krono.h>

class Lens
{
public:
	virtual ~Lens();

	virtual krono::Matrix4f GetProjectionMatrix() const = 0;
protected:
	Lens(float nearPlane, float farPlane); 
	
	float mNear;
	float mFar;
	//float mWhereEeeeeevarYouAre;
private:
};

class PerspectiveLens : public Lens
{
public:
	PerspectiveLens(float nearPlane, float farPlane, const krono::Radiansf& verticalFov);

	virtual krono::Matrix4f GetProjectionMatrix() const;
private:
	krono::Radiansf mVerticalFov;
};

class OrthographicLens : public Lens
{
public:
	OrthographicLens(float nearPlane, float farPlane, float height);

	virtual krono::Matrix4f GetProjectionMatrix() const;
private:
	float mHeight;
};
