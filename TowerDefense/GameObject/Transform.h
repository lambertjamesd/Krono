#pragma once

#include "Component.h"
#include <Krono.h>

class Transform : public Component
{
public:
	typedef std::shared_ptr<Transform> Ptr;
	typedef std::weak_ptr<Transform> Ref;

	Transform(GameObject* parentGameObject);
	~Transform(void);

	const Quaternionf& GetLocalOrientation() const;
	const Vector3f& GetLocalPosition() const;
	const Vector3f& GetLocalScale() const;

	void SetLocalOrientation(const Quaternionf& value);
	void SetLocalPosition(const Vector3f& value);
	void SetLocalScale(const Vector3f& value);

	void SetParent(const Transform::Ref& value);
	const Transform::Ref& GetParent() const;

	const Matrix4f& LocalTransform() const;
	const Matrix4f& WorldTransform() const;
private:
	void RemoveChild(Transform* child);
	void AddChild(Transform* child);

	void SetIsTransformDirty();
	void SetIsWorldTransformDirty();

	std::vector<Transform*> mChildren;
	Transform::Ref mParent;

	bool mIsTransformDirty;
	Matrix4f mTransform;

	bool mIsWorldTransformDirty;
	Matrix4f mWolrdTransform;

	Quaternionf mOrientation;
	Vector3f mPosition;
	Vector3f mScale;
};

