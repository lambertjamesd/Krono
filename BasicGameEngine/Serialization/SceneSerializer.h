#pragma once

#include <string>
#include <Krono.h>
#include "Scene/Scene.h"
#include "ComponentFactory.h"
#include <iostream>
#include <functional>

#include "SceneDeserializer.h"

namespace kge
{

class SceneSerializer
{
public:
	~SceneSerializer(void);

	void SerializeScene(Scene& scene);
	void SerializePrefab(GameObject& gameObject);

	virtual void WriteNull() = 0;
	virtual void WriteFloat(float value) = 0;
	virtual void WriteInt(int integer) = 0;
	virtual void WriteString(const std::string& value) = 0;
	virtual void WriteBool(bool value) = 0;
	virtual void WriteVector3(const krono::Vector3f& value) = 0;
	virtual void WriteQuaternion(const krono::Quaternionf& value) = 0;


	virtual void BeginObject() = 0;
	virtual void WriteKey(const std::string& name) = 0;
	virtual void EndObject() = 0;

	virtual void BeginArray() = 0;
	virtual void EndArray() = 0;
	
	void WriteNull(const std::string& name);
	void WriteFloat(const std::string& name, float value);
	void WriteInt(const std::string& name, int integer);
	void WriteString(const std::string& name, const std::string& value);
	void WriteBool(const std::string& name, bool value);
	void WriteVector3(const std::string& name, const krono::Vector3f& value);
	void WriteQuaternion(const std::string& name, const krono::Quaternionf& value);

	void BeginObject(const std::string& name);
	void BeginArray(const std::string& name);
	
	void WriteComponentReference(Component::Ref component);
	void WriteComponentReference(const std::string& name, Component::Ref component);
	void WriteGameObjectReference(GameObject::Ref gameObject);
	void WriteGameObjectReference(const std::string& name, GameObject::Ref gameObject);
	void WriteResource(const krono::Resource::Ptr& resource);
	void WriteResource(const std::string& name, const krono::Resource::Ptr& resource);

	void WriteSharedPointer(const std::string& name, const krono::Object::Ptr& object, std::function<void(SceneSerializer& serializer, const krono::Object::Ptr& object)> serializeCallback);
	void WriteSharedPointer(const krono::Object::Ptr& object, std::function<void(SceneSerializer& serializer, const krono::Object::Ptr& object)> serializeCallback);
	
protected:
	virtual void Finish() = 0;

	void WriteScene(Scene& scene);
	void WriteGameObject(GameObject& gameObject);
	void WriteComponent(Component& component);

	std::string GetObjectID(GameObject& gameObject);
	std::string GetComponentID(Component& component);

	SceneSerializer(ComponentFactory& componentFactory, std::ostream& output);

	ComponentFactory& mComponentFactory;
	std::ostream& mOutput;
private:
	void CreateIDMapping(const void* address);
	void MapGameObject(GameObject& gameObject);

	size_t mCurrentID;
	std::map<const void*, std::string> mIDMapping;
	static const std::string gInvalidID;
};

}
