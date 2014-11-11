#pragma once

#include <Krono.h>

#include <memory>

#include "Serialization/ISerializable.h"

namespace kge
{

class GameObject;

class Component : public krono::Object, public ISerializable
{
public:
	virtual ~Component(void);

	typedef std::shared_ptr<Component> Ptr;
	typedef std::weak_ptr<Component> Ref;

	GameObject& GetGameObject();
	krono::ResourceManager& GetResourceManager();
	const krono::InputState& GetInputState() const;
	
	virtual void Serialize(SceneSerializer& serializer);
	virtual void Deserialize(SceneDeserializer& deserializer);
	
	Ref GetWeakPointer() const;
protected:
	Component(GameObject& parentGameObject);
	
	GameObject &mGameObject;
private:
	friend class GameObject;

	Component(const Component& other);
	Component& operator=(const Component& other);
	Ref mSelfReference;
};

}
