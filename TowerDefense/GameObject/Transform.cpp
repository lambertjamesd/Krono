#include "Transform.h"
#include <algorithm>

Transform::Transform(GameObject* parentGameObject) :
	Component(parentGameObject),
	mIsTransformDirty(false),
	mTransform(Matrix4f::Identity()),
	mIsWorldTransformDirty(false),
	mWolrdTransform(Matrix4f::Identity()),

	mOrientation(),
	mPosition(),
	mScale(1.0f, 1.0f, 1.0f)
{

}


Transform::~Transform(void)
{
	for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
	{
		(*it)->mParent.reset();
		(*it)->SetParent(mParent);
	}
}

const Quaternionf& Transform::GetLocalOrientation() const
{
	return mOrientation;
}

const Vector3f& Transform::GetLocalPosition() const
{
	return mPosition;
}

const Vector3f& Transform::GetLocalScale() const
{
	return mScale;
}

void Transform::SetLocalOrientation(const Quaternionf& value)
{
	mOrientation = value;
	SetIsTransformDirty();
}

void Transform::SetLocalPosition(const Vector3f& value)
{
	mPosition = value;
	SetIsTransformDirty();
}

void Transform::SetLocalScale(const Vector3f& value)
{
	mScale = value;
	SetIsTransformDirty();
}

void Transform::SetParent(const Transform::Ref& value)
{
	if (!mParent.expired())
	{
		mParent.lock()->RemoveChild(this);
	}

	mParent = value;

	if (!mParent.expired())
	{
		mParent.lock()->AddChild(this);
	}

	SetIsWorldTransformDirty();
}

const Transform::Ref& Transform::GetParent() const
{
	return mParent;
}

const Matrix4f& Transform::LocalTransform() const
{
	if (mIsTransformDirty)
	{
		const_cast<Matrix4f&>(mTransform) = Matrix4f::Translation(mPosition) * Matrix4f::Scale(mScale) * Matrix4f::Rotation(mOrientation);
		const_cast<bool&>(mIsTransformDirty) = false;
	}

	return mTransform;
}

const Matrix4f& Transform::WorldTransform() const
{
	if (mIsWorldTransformDirty)
	{
		if (mParent.expired())
		{
			const_cast<Matrix4f&>(mWolrdTransform) = LocalTransform();
		}
		else
		{
			const_cast<Matrix4f&>(mWolrdTransform) = mParent.lock()->WorldTransform() * LocalTransform();
		}

		const_cast<bool&>(mIsWorldTransformDirty) = false;
	}

	return mWolrdTransform;
}

void Transform::RemoveChild(Transform* child)
{
	mChildren.erase(std::find(mChildren.begin(), mChildren.end(), child));
}

void Transform::AddChild(Transform* child)
{
	mChildren.push_back(child);
}

void Transform::SetIsTransformDirty()
{
	mIsTransformDirty = true;
	SetIsWorldTransformDirty();
}

void Transform::SetIsWorldTransformDirty()
{
	if (!mIsWorldTransformDirty)
	{
		mIsWorldTransformDirty = true;

		for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->SetIsWorldTransformDirty();
		}
	}
}