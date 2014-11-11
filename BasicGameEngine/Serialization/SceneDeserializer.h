#pragma once

#include <iostream>
#include <Krono.h>
#include "Scene/Scene.h"
#include "ComponentFactory.h"
#include <functional>

namespace kge
{

class SceneDeserializer
{
public:
	~SceneDeserializer(void);

	Scene::Ptr DeserializeScene(Game& game);
	GameObject::Ref DeserializePrefab(Scene& scene);

	virtual float ReadFloat(float defaultValue) = 0;
	virtual int ReadInt(int defaultValue) = 0;
	virtual std::string ReadString(const std::string& defaultValue) = 0;
	virtual bool ReadBool(bool defaultValue) = 0;
	virtual krono::Vector3f ReadVector3(const krono::Vector3f& defaultValue) = 0;
	virtual krono::Quaternionf ReadQuaternion(const krono::Quaternionf& defaultValue) = 0;

	virtual void BeginObject() = 0;
	virtual void ReadKey(const std::string& key) = 0;
	virtual void EndObject() = 0;

	virtual size_t BeginArray() = 0;
	virtual void EndArray() = 0;

	float ReadFloat(const std::string& key, float defaultValue);
	int ReadInt(const std::string& key, int defaultValue);
	std::string ReadString(const std::string& key, const std::string& defaultValue);
	bool ReadBool(const std::string& key, bool defaultValue);
	krono::Vector3f ReadVector3(const std::string& key, const krono::Vector3f& defaultValue);
	krono::Quaternionf ReadQuaternion(const std::string& key, const krono::Quaternionf& defaultValue);

	void BeginObject(const std::string& key);
	size_t BeginArray(const std::string& key);
	
	template <typename T>
	std::weak_ptr<T> ReadComponentReference()
	{
		std::string idString = ReadString("");

		if (idString.length() == 0)
		{
			return std::weak_ptr<T>();
		}
		else
		{
			return std::dynamic_pointer_cast<T>(mComponentMapping[idString]);
		}
	}
	
	template <typename T>
	std::weak_ptr<T> ReadComponentReference(const std::string& key)
	{
		ReadKey(key);
		return ReadComponentReference<T>();
	}

	GameObject::Ref ReadGameObjectReference();
	GameObject::Ref ReadGameObjectReference(const std::string& key);

	template <typename T>
	std::shared_ptr<T> ReadResource()
	{
		BeginObject();

		std::string filename = ReadString("source", "");

		std::shared_ptr<T> result;

		if (filename.length() > 0)
		{
			result = mResourceManager.LoadResource<T>(filename);
		}

		EndObject();

		return result;
	}
	
	template <typename T>
	std::shared_ptr<T> ReadResource(const std::string& key)
	{
		ReadKey(key);
		return ReadResource<T>();
	}

	template <typename T>
	std::shared_ptr<T> ReadSharedPointer(std::function<std::shared_ptr<T>(SceneDeserializer&)> objectConstructor)
	{
		BeginObject();

		std::shared_ptr<T> result;

		std::string id = ReadString("id", "");

		auto existingObject = mSharedMapping.find(id);

		if (existingObject == mSharedMapping.end())
		{
			BeginObject("data");
			mSharedMapping[id] = result = objectConstructor(*this);
			EndObject();
		}
		else
		{
			result = std::dynamic_pointer_cast<T>(existingObject->second);
		}

		EndObject();

		return result;
	}

	template <typename T>
	std::shared_ptr<T> ReadSharedPointer(const std::string& name, std::function<std::shared_ptr<T>(SceneDeserializer&)> objectConstructor)
	{
		ReadKey(name);
		return ReadSharedPointer(objectConstructor);
	}

protected:
	virtual void PushState() = 0;
	virtual void PopState() = 0;

	SceneDeserializer(ComponentFactory& componentFactory, krono::ResourceManager& resourceManager);

	void CreateScene(Scene& scene);
	GameObject::Ref CreateGameObject(Scene& scene);
	void CreateComponent(GameObject& gameObject);

	void PopulateScene();
	void PopulateGameObject();
	void PopulateComponent();
private:
	ComponentFactory& mComponentFactory;
	krono::ResourceManager& mResourceManager;
	
	std::map<std::string, GameObject::Ptr> mGameObjectMapping;
	std::map<std::string, Component::Ptr> mComponentMapping;
	std::map<std::string, krono::Object::Ptr> mSharedMapping;
};

}