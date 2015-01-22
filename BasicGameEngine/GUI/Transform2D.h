#pragma once

#include <Krono.h>

namespace kge
{

class Transform2D
{
public:
	Transform2D(void);
	~Transform2D(void);

	void TransformPoint(const krono::Vector3f& input, krono::Vector3f& output);

	void ConcatTransform(const Transform2D& other);
	void ConcatTransform(const krono::Vector2f& position, const krono::Vector2f& right, const krono::Vector2f& scale);
private:
	krono::Matrix4f mMatrix;
};

}