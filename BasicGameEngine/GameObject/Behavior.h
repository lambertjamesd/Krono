#pragma once

#include "Component.h"
#include "Scene/UpdateManager.h"
namespace kge
{

class Behavior : public Component, public Updatable
{
public:
	~Behavior(void);

	virtual void Update(float deltaTime);
protected:
	Behavior(GameObject& parentGameObject);
};

}
