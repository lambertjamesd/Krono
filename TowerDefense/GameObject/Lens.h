#pragma once

#include <Krono.h>

class Lens
{
public:
	virtual ~Lens();

	virtual krono::Matrix4f GetProjectionMatrix() const = 0;
protected:
	Lens(float near, float far); 
	
	float mNear;
	float mFar;
	//float mWhereEeeeeevarYouAre;
private:
};

class PerspectiveLens : public Lens
{
public:
	PerspectiveLens(float near, float far, const krono::Radiansf& verticalFov);

	virtual krono::Matrix4f GetProjectionMatrix() const;
private:
	float mVerticalFov;
};

class OrthographicLens : public Lens
{
public:
	OrthographicLens(float near, float far, float height);

	virtual krono::Matrix4f GetProjectionMatrix() const;
private:
	float mHeight;
};
