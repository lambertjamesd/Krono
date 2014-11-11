#pragma once

#include "GameObject/Component.h"
#include "GameObject/GameObject.h"
#include <memory>
#include <map>
#include <string>

namespace kge
{

class ComponentConstructor
{
public:
	virtual Component::Ref BuildComponent(GameObject& gameObject) const = 0;
private:
};

template <typename T>
class DefaultComponentConstructor : public ComponentConstructor
{
public:
	virtual Component::Ref BuildComponent(GameObject& gameObject) const
	{
		return gameObject.AddComponent<T>();
	}
private:
};

class TransformComponentConstructor : public ComponentConstructor
{
public:
	virtual Component::Ref BuildComponent(GameObject& gameObject) const;
private:
};

class ComponentFactory
{
public:
	ComponentFactory(void);
	~ComponentFactory(void);

	template <typename T>
	void AddConstructor(const std::string& componentName, std::unique_ptr<ComponentConstructor> constructor)
	{
		mComponentConstructor[componentName] = move(constructor);
		mClassNameMapping[typeid(T).hash_code()] = componentName;
	}

	template <typename T>
	void AddDefaultConstructor(const std::string& componentName)
	{
		mComponentConstructor[componentName] = std::unique_ptr<DefaultComponentConstructor<T> >(new DefaultComponentConstructor<T>());
		mClassNameMapping[typeid(T).hash_code()] = componentName;
	}

	bool HasComponentConstructor(const std::string& name) const;
	Component::Ref CreateComponent(const std::string& name, GameObject& gameObject) const;
	
	bool HasComponentConstructor(const std::type_info& typeInfo) const;
	std::string GetComponentName(const std::type_info& typeInfo) const;
private:
	std::map<std::string, std::unique_ptr<ComponentConstructor> > mComponentConstructor;
	std::map<size_t, std::string> mClassNameMapping;
};

}