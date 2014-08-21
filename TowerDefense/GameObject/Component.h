#pragma once

#include <memory>

class GameObject;

class Component
{
public:
	virtual ~Component(void);

	typedef std::shared_ptr<Component> Ptr;
	typedef std::weak_ptr<Component> Ref;
protected:
	Component(GameObject* parentGameObject);
	
	GameObject *mGameObject;
private:
	Component(const Component& other);
	Component& operator=(const Component& other);
};

