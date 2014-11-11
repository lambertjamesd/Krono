#include "SceneDeserializer.h"

#include "Scene/Game.h"

using namespace std;

namespace kge
{

SceneDeserializer::SceneDeserializer(ComponentFactory& componentFactory, krono::ResourceManager& resourceManager) :
	mComponentFactory(componentFactory),
	mResourceManager(resourceManager)
{

}


SceneDeserializer::~SceneDeserializer(void)
{
}

Scene::Ptr SceneDeserializer::DeserializeScene(Game& game)
{
	Scene::Ptr result = game.CreateScene();

	PushState();
	CreateScene(*result);
	PopState();

	PopulateScene();

	return result;
}

GameObject::Ref SceneDeserializer::DeserializePrefab(Scene& scene)
{
	size_t objectCount = BeginArray();
	
	PushState();

	for (size_t i = 0; i < objectCount; ++i)
	{
		CreateGameObject(scene);
	}
	
	PopState();
	
	for (size_t i = 0; i < objectCount; ++i)
	{
		PopulateGameObject();
	}

	EndArray();

	return mGameObjectMapping["1"];
}

float SceneDeserializer::ReadFloat(const std::string& key, float defaultValue)
{
	ReadKey(key);
	return ReadFloat(defaultValue);
}

int SceneDeserializer::ReadInt(const std::string& key, int defaultValue)
{
	ReadKey(key);
	return ReadInt(defaultValue);
}

std::string SceneDeserializer::ReadString(const std::string& key, const std::string& defaultValue)
{
	ReadKey(key);
	return ReadString(defaultValue);
}

bool SceneDeserializer::ReadBool(const std::string& key, bool defaultValue)
{
	ReadKey(key);
	return ReadBool(defaultValue);
}

krono::Vector3f SceneDeserializer::ReadVector3(const std::string& key, const krono::Vector3f& defaultValue)
{
	ReadKey(key);
	return ReadVector3(defaultValue);
}

krono::Quaternionf SceneDeserializer::ReadQuaternion(const std::string& key, const krono::Quaternionf& defaultValue)
{
	ReadKey(key);
	return ReadQuaternion(defaultValue);
}

void SceneDeserializer::BeginObject(const std::string& key)
{
	ReadKey(key);
	BeginObject();
}

size_t SceneDeserializer::BeginArray(const std::string& key)
{
	ReadKey(key);
	return BeginArray();
}

GameObject::Ref SceneDeserializer::ReadGameObjectReference()
{
	return mGameObjectMapping[ReadString("")];
}

GameObject::Ref SceneDeserializer::ReadGameObjectReference(const std::string& key)
{
	ReadKey(key);
	return ReadGameObjectReference();
}

void SceneDeserializer::CreateScene(Scene& scene)
{
	BeginObject();

	size_t objectCount = BeginArray("gameObjects");

	for (size_t i = 0; i < objectCount; ++i)
	{
		CreateGameObject(scene);
	}

	EndArray();

	EndObject();
}

GameObject::Ref SceneDeserializer::CreateGameObject(Scene& scene)
{
	BeginObject();

	GameObject::Ptr result = scene.CreateGameObject().lock();

	mGameObjectMapping[ReadString("id", "")] = result;

	size_t componentCount = BeginArray("components");

	for (size_t i = 0; i < componentCount; ++i)
	{
		CreateComponent(*result);
	}

	EndArray();

	EndObject();

	return result;
}

void SceneDeserializer::CreateComponent(GameObject& gameObject)
{
	BeginObject();

	string className = ReadString("class", "");

	if (mComponentFactory.HasComponentConstructor(className))
	{
		mComponentMapping[ReadString("id", "")] = mComponentFactory.CreateComponent(className, gameObject).lock();
	}

	EndObject();
}

void SceneDeserializer::PopulateScene()
{
	BeginObject();

	size_t objectCount = BeginArray("gameObjects");

	for (size_t i = 0; i < objectCount; ++i)
	{
		PopulateGameObject();
	}

	EndArray();

	EndObject();
}

void SceneDeserializer::PopulateGameObject()
{
	BeginObject();

	size_t componentCount = BeginArray("components");

	for (size_t i = 0; i < componentCount; ++i)
	{
		PopulateComponent();
	}

	EndArray();

	EndObject();
}

void SceneDeserializer::PopulateComponent()
{
	BeginObject();

	string id = ReadString("id", "");

	if (id != "")
	{
		Component::Ptr component = mComponentMapping[id];

		if (component != NULL)
		{
			BeginObject("data");
			component->Deserialize(*this);
			EndObject();
		}
	}

	EndObject();
}

}