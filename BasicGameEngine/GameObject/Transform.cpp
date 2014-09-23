#include "Transform.h"
#include <algorithm>
#include <iostream>

namespace kge
{

using namespace krono;

Transform::Transform(GameObject& parentGameObject) :
	Component(parentGameObject),
	mIsTransformDirty(false),
	mTransform(Matrix4f::Identity()),
	mIsInverseTransformDirty(false),
	mInverseTransform(Matrix4f::Identity()),
	mIsWorldTransformDirty(false),
	mWorldTransform(Matrix4f::Identity()),
	mIsInverseWorldTransformDirty(false),
	mInverseWorldTransform(Matrix4f::Identity()),

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

	ClearParent();
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

void Transform::ClearParent()
{
	if (!mParent.expired())
	{
		mParent.lock()->RemoveChild(this);
		mParent.reset();
		SetIsWorldTransformDirty();
	}
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

const Matrix4f& Transform::GetLocalTransform() const
{
	if (mIsTransformDirty)
	{
		const_cast<Matrix4f&>(mTransform) = Matrix4f::Translation(mPosition)* Matrix4f::Rotation(mOrientation) * Matrix4f::Scale(mScale);
		const_cast<bool&>(mIsTransformDirty) = false;
	}

	return mTransform;
}

const krono::Matrix4f& Transform::GetInverseLocalTransform() const
{
	if (mIsInverseTransformDirty)
	{
		const_cast<Matrix4f&>(mInverseTransform) = GetLocalTransform().Inverse();
		const_cast<bool&>(mIsInverseTransformDirty) = false;
	}

	return mInverseTransform;
}

const Matrix4f& Transform::GetWorldTransform() const
{
	if (mIsWorldTransformDirty)
	{
		if (mParent.expired())
		{
			const_cast<Matrix4f&>(mWorldTransform) = GetLocalTransform();
		}
		else
		{
			const_cast<Matrix4f&>(mWorldTransform) = mParent.lock()->GetWorldTransform() * GetLocalTransform();
		}

		const_cast<bool&>(mIsWorldTransformDirty) = false;
	}

	return mWorldTransform;
}

const krono::Matrix4f& Transform::GetInverseWorldTransform() const
{
	if (mIsInverseWorldTransformDirty)
	{
		const_cast<Matrix4f&>(mInverseWorldTransform) = GetWorldTransform().Inverse();
		const_cast<bool&>(mIsInverseWorldTransformDirty) = false;
	}

	return mInverseWorldTransform;
}

void Transform::RemoveChild(Transform* child)
{
	mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), child), mChildren.end());
}

void Transform::AddChild(Transform* child)
{
	mChildren.push_back(child);
}

void Transform::SetIsTransformDirty()
{
	mIsTransformDirty = true;
	mIsInverseTransformDirty = true;
	SetIsWorldTransformDirty();
}

void Transform::SetIsWorldTransformDirty()
{
	if (!mIsWorldTransformDirty)
	{
		mIsWorldTransformDirty = true;
		mIsInverseWorldTransformDirty = true;

		for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->SetIsWorldTransformDirty();
		}
	}
}

}
