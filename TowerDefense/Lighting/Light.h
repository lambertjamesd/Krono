#pragma once

#include "GameObject/Component.h"
#include "Render/Renderable.h"

class Light : public Component
{
public:
	typedef std::shared_ptr<Light> Ptr;
	typedef std::weak_ptr<Light> Ref;

	Light(GameObject& parentGameObject);
	~Light(void);
private:
};

