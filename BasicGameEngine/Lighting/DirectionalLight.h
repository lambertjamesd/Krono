#pragma once

#include "Light.h"
#include <Krono.h>

namespace kge
{

class DirectionalLight : public Light, public Renderable
{
public:
	typedef std::shared_ptr<DirectionalLight> Ptr;
	typedef std::weak_ptr<DirectionalLight> Ref;

	DirectionalLight(GameObject& parentGameObject);
	~DirectionalLight(void);

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

	void SetDirection(const krono::Vector3f& direction);
	void SetColor(const krono::Colorf& value);
protected:
	krono::Entity& mEntity;
};

}
