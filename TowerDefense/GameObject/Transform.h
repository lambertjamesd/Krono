#pragma once

#include "Component.h"
#include <Krono.h>

class Transform : public Component
{
public:
	Transform(GameObject* parentGameObject);
	~Transform(void);
private:
};

