#include "ComponentFactory.h"
#include <Krono.h>
#include "Gameobject/Renderer.h"
#include "Gameobject/Camera.h"
#include "Scripting/LuaBehavior.h"

using namespace krono;

namespace kge
{
	
Component::Ref TransformComponentConstructor::BuildComponent(GameObject& gameObject) const
{
	return gameObject.GetTransform();
}

ComponentFactory::ComponentFactory(void)
{
	AddConstructor<Transform>("Transform", std::unique_ptr<TransformComponentConstructor>(new TransformComponentConstructor()));
	AddDefaultConstructor<Renderer>("Renderer");
	AddDefaultConstructor<Camera>("Camera");
	AddDefaultConstructor<LuaBehavior>("LuaBehavior");
}

ComponentFactory::~ComponentFactory(void)
{
}

bool ComponentFactory::HasComponentConstructor(const std::string& name) const
{
	auto constructor = mComponentConstructor.find(name);
	return constructor != mComponentConstructor.end();
}

Component::Ref ComponentFactory::CreateComponent(const std::string& name, GameObject& gameObject) const
{
	auto constructor = mComponentConstructor.find(name);

	if (constructor == mComponentConstructor.end())
	{
		throw Exception("Could not find component constructor");
	}
	else
	{
		return constructor->second->BuildComponent(gameObject);
	}
}

bool ComponentFactory::HasComponentConstructor(const std::type_info& typeInfo) const
{
	auto constructor = mClassNameMapping.find(typeInfo.hash_code());
	return constructor != mClassNameMapping.end();
}

std::string ComponentFactory::GetComponentName(const std::type_info& typeInfo) const
{
	auto constructor = mClassNameMapping.find(typeInfo.hash_code());

	if (constructor == mClassNameMapping.end())
	{
		throw Exception("Could not find component constructor");
	}
	else
	{
		return constructor->second;
	}
}

}