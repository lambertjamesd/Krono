
#include "GameObject.h"
#include "Scene/Scene.h"
namespace kge
{

GameObject::GameObject(Scene* parentScene) :
	mScene(parentScene),
	mTransform(AddComponent<Transform>().lock())
{

}


GameObject::~GameObject(void)
{

}

const Transform::Ptr GameObject::GetTransform() const
{
	return mTransform;
}

Scene& GameObject::GetScene()
{
	return *mScene;
}

}
