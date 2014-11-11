#pragma once

namespace kge
{

class SceneSerializer;
class SceneDeserializer;

class ISerializable
{
public:
	virtual void Serialize(SceneSerializer& serializer) = 0;
	virtual void Deserialize(SceneDeserializer& deserializer) = 0;
};

}