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

	template <typename T>
	void SetVariable(const std::string& name, const T& value)
	{
		mEntity.GetMaterial(0)->SetVariable<T>(name, value);
	}

	template <typename T>
	void SetArray(const std::string& name, const T* value, size_t count)
	{
		mEntity.GetMaterial(0)->SetArrayVariable<T>(name, value, count);
	}
protected:
	krono::Entity& mEntity;
};

}
