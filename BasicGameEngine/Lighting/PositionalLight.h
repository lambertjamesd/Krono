#pragma once

#include "Light.h"
#include <Krono.h>

namespace kge
{

class PositionalLight : public Light, public Renderable
{
public:
	PositionalLight(GameObject& parentGameObject);
	~PositionalLight(void);

	virtual void PreRender();
private:
	krono::Entity& mEntity;
};

}
