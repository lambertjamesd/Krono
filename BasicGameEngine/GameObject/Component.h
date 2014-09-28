#pragma once

#include <Krono.h>

#include <memory>
namespace kge
{

class GameObject;

class Component
{
public:
	virtual ~Component(void);

	typedef std::shared_ptr<Component> Ptr;
	typedef std::weak_ptr<Component> Ref;

	GameObject& GetGameObject();
	krono::ResourceManager& GetResourceManager();
	const krono::InputState& GetInputState() const;
protected:
	Component(GameObject& parentGameObject);
	
	GameObject &mGameObject;
private:
	Component(const Component& other);
	Component& operator=(const Component& other);
};

}
