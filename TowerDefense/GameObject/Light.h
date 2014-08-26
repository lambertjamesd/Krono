#pragma once

#include "Component.h"
#include "Render/Renderable.h"
#include "Transform.h"

class Light : public Component, public Renderable
{
public:
	typedef std::shared_ptr<Light> Ptr;
	typedef std::weak_ptr<Light> Ref;

	Light(GameObject& parentGameObject);
	~Light(void);
	
	virtual void PreRender();

private:
	krono::Entity& mEntity;
};

