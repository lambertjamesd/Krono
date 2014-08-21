#pragma once

#include "Component.h"
#include <Krono.h>

class Transform : public Component
{
public:
	typedef std::shared_ptr<Transform> Ptr;
	typedef std::weak_ptr<Transform> Ref;

	Transform(GameObject& parentGameObject);
	~Transform(void);

	const krono::Quaternionf& GetLocalOrientation() const;
	const krono::Vector3f& GetLocalPosition() const;
	const krono::Vector3f& GetLocalScale() const;

	void SetLocalOrientation(const krono::Quaternionf& value);
	void SetLocalPosition(const krono::Vector3f& value);
	void SetLocalScale(const krono::Vector3f& value);

	void ClearParent();
	void SetParent(const Transform::Ref& value);
	const Transform::Ref& GetParent() const;

	const krono::Matrix4f& GetLocalTransform() const;
	const krono::Matrix4f& GetWorldTransform() const;
private:
	void RemoveChild(Transform* child);
	void AddChild(Transform* child);

	void SetIsTransformDirty();
	void SetIsWorldTransformDirty();

	std::vector<Transform*> mChildren;
	Transform::Ref mParent;

	bool mIsTransformDirty;
	krono::Matrix4f mTransform;

	bool mIsWorldTransformDirty;
	krono::Matrix4f mWolrdTransform;

	krono::Quaternionf mOrientation;
	krono::Vector3f mPosition;
	krono::Vector3f mScale;
};

