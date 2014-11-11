
#include "GameObject.h"
#include "Scene/Scene.h"
namespace kge
{

GameObject::GameObject(Scene& parentScene) :
	mScene(parentScene),
	mTransform(AddComponent<Transform>().lock())
{

}


GameObject::~GameObject(void)
{

}

size_t GameObject::GetComponentCount() const
{
	return mComponents.size();
}

Component::Ref GameObject::GetComponent(size_t index) const
{
	return mComponents[index];
}

const Transform::Ptr GameObject::GetTransform() const
{
	return mTransform;
}

Scene& GameObject::GetScene()
{
	return mScene;
}

GameObject::Ref GameObject::GetWeakPointer() const
{
	return mSelfReference;
}

void GameObject::VisitSelfAndDecendants(IteratorCallback callback)
{
	callback(*this);

	for (size_t i = 0; i < mTransform->GetChildrenCount(); ++i)
	{
		mTransform->GetChild(i).GetGameObject().VisitSelfAndDecendants(callback);
	}
}

void GameObject::Destroy()
{
	mScene.RemoveGameObject(*this);
}

}
