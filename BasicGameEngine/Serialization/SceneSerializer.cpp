#include "SceneSerializer.h"
#include <sstream>

using namespace std;

namespace kge
{

SceneSerializer::SceneSerializer(ComponentFactory& componentFactory, std::ostream& output) :
	mComponentFactory(componentFactory),
	mOutput(output),
	mCurrentID(0)
{
}


SceneSerializer::~SceneSerializer(void)
{
}

void SceneSerializer::SerializeScene(Scene& scene)
{
	for (size_t i = 0; i < scene.GetGameObjectCount(); ++i)
	{
		MapGameObject(*scene.GetGameObject(i).lock());
	}

	WriteScene(scene);

	Finish();
}

void SceneSerializer::SerializePrefab(GameObject& gameObject)
{
	gameObject.VisitSelfAndDecendants([&](GameObject& visitedObject) {
		MapGameObject(visitedObject);
	});

	BeginArray();
	
	gameObject.VisitSelfAndDecendants([&](GameObject& visitedObject) {
		WriteGameObject(visitedObject);
	});

	EndArray();

	Finish();
}

void SceneSerializer::WriteNull(const std::string& name)	
{
	WriteKey(name);
	WriteNull();
}

void SceneSerializer::WriteFloat(const std::string& name, float value)
{
	WriteKey(name);
	WriteFloat(value);
}

void SceneSerializer::WriteInt(const std::string& name, int integer)
{
	WriteKey(name);
	WriteInt(integer);
}

void SceneSerializer::WriteString(const std::string& name, const std::string& value)
{
	WriteKey(name);
	WriteString(value);
}

void SceneSerializer::WriteBool(const std::string& name, bool value)
{
	WriteKey(name);
	WriteBool(value);
}

void SceneSerializer::WriteVector3(const std::string& name, const krono::Vector3f& value)
{
	WriteKey(name);
	WriteVector3(value);
}

void SceneSerializer::WriteQuaternion(const std::string& name, const krono::Quaternionf& value)
{
	WriteKey(name);
	WriteQuaternion(value);
}

void SceneSerializer::BeginObject(const std::string& name)
{
	WriteKey(name);
	BeginObject();
}

void SceneSerializer::BeginArray(const std::string& name)
{
	WriteKey(name);
	BeginArray();
}

void SceneSerializer::WriteComponentReference(Component::Ref component)
{
	if (component.expired())
	{
		WriteNull();
	}
	else
	{
		std::string id = GetComponentID(*component.lock());

		if (id == gInvalidID)
		{
			WriteNull();
		}
		else
		{
			WriteString(id);
		}
	}
}

void SceneSerializer::WriteComponentReference(const std::string& name, Component::Ref component)
{
	WriteKey(name);
	WriteComponentReference(component);
}

void SceneSerializer::WriteGameObjectReference(GameObject::Ref gameObject)
{
	if (gameObject.expired())
	{
		WriteNull();
	}
	else
	{
		std::string id = GetObjectID(*gameObject.lock());

		if (id == gInvalidID)
		{
			WriteNull();
		}
		else
		{
			WriteString(id);
		}
	}
}

void SceneSerializer::WriteGameObjectReference(const std::string& name, GameObject::Ref gameObject)
{
	WriteKey(name);
	WriteGameObjectReference(gameObject);
}

void SceneSerializer::WriteResource(const krono::Resource::Ptr& resource)
{
	if (resource == NULL)
	{
		WriteNull();
	}
	else
	{
		BeginObject();
		WriteString("source", resource->GetSource());
		EndObject();
	}
}

void SceneSerializer::WriteResource(const std::string& name, const krono::Resource::Ptr& resource)
{
	WriteKey(name);
	WriteResource(resource);
}

void SceneSerializer::WriteSharedPointer(const std::string& name, const krono::Object::Ptr& object, std::function<void(SceneSerializer& serializer, const krono::Object::Ptr& object)> serializeCallback)
{
	WriteKey(name);
	WriteSharedPointer(object, serializeCallback);
}

void SceneSerializer::WriteSharedPointer(const krono::Object::Ptr& object, std::function<void(SceneSerializer& serializer, const krono::Object::Ptr& object)> serializeCallback)
{
	auto existingID = mIDMapping.find(object.get());
		
	BeginObject();

	if (existingID == mIDMapping.end())
	{
		CreateIDMapping(object.get());
		WriteString("id", mIDMapping[object.get()]);

		BeginObject("data");

		serializeCallback(*this, object);

		EndObject();
	}
	else
	{
		WriteString("id", existingID->second);
	}

	EndObject();
}

void SceneSerializer::WriteScene(Scene& scene)
{
	BeginObject();

	BeginArray("gameObjects");

	for (size_t i = 0; i < scene.GetGameObjectCount(); ++i)
	{
		WriteGameObject(*scene.GetGameObject(i).lock());
	}

	EndArray();

	EndObject();
}

void SceneSerializer::WriteGameObject(GameObject& gameObject)
{
	BeginObject();

	WriteString("id", GetObjectID(gameObject));

	BeginArray("components");

	for (size_t i = 0; i < gameObject.GetComponentCount(); ++i)
	{
		WriteComponent(*gameObject.GetComponent(i).lock());
	}

	EndArray();

	EndObject();
}

void SceneSerializer::WriteComponent(Component& component)
{
	if (mComponentFactory.HasComponentConstructor(typeid(component)))
	{
		BeginObject();
	
		WriteString("id", GetComponentID(component));
		WriteString("class", mComponentFactory.GetComponentName(typeid(component)));

		BeginObject("data");

		component.Serialize(*this);

		EndObject();

		EndObject();
	}
}

std::string SceneSerializer::GetObjectID(GameObject& gameObject)
{
	auto result = mIDMapping.find(&gameObject);
	
	if (result == mIDMapping.end())
	{
		return gInvalidID;
	}
	else
	{
		return result->second;
	}
}

std::string SceneSerializer::GetComponentID(Component& component)
{
	auto result = mIDMapping.find(&component);
	
	if (result == mIDMapping.end())
	{
		return gInvalidID;
	}
	else
	{
		return result->second;
	}
}

void SceneSerializer::CreateIDMapping(const void* address)
{
	++mCurrentID;
	std::ostringstream idMaker;
	idMaker << mCurrentID;
	mIDMapping[address] = idMaker.str();
}

void SceneSerializer::MapGameObject(GameObject& gameObject)
{
	CreateIDMapping(&gameObject);

	for (size_t i = 0; i < gameObject.GetComponentCount(); ++i)
	{
		CreateIDMapping(gameObject.GetComponent(i).lock().get());
	}
}

const std::string SceneSerializer::gInvalidID("");

}