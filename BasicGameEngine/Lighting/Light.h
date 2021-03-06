#pragma once

#include "GameObject/Component.h"
#include "Render/Renderable.h"

namespace kge
{

class Light : public Component
{
public:
	typedef std::shared_ptr<Light> Ptr;
	typedef std::weak_ptr<Light> Ref;

	Light(GameObject& parentGameObject);
	virtual ~Light(void);
private:
};

}
