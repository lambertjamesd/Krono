#pragma once

#include "SceneDeserializer.h"
#include "SceneSerializer.h"

#include <Krono.h>
#include <iostream>
#include <vector>
#include <string>

namespace kge
{

class SceneJsonDeserializer : public SceneDeserializer
{
public:
	SceneJsonDeserializer(ComponentFactory& componentFactory, krono::ResourceManager& resourceManager, std::istream& input);
	~SceneJsonDeserializer(void);

	virtual float ReadFloat(float defaultValue);
	virtual int ReadInt(int defaultValue);
	virtual std::string ReadString(const std::string& defaultValue);
	virtual bool ReadBool(bool defaultValue);
	virtual krono::Vector3f ReadVector3(const krono::Vector3f& defaultValue);
	virtual krono::Quaternionf ReadQuaternion(const krono::Quaternionf& defaultValue);

	virtual void BeginObject();
	virtual void ReadKey(const std::string& key);
	virtual void EndObject();

	virtual size_t BeginArray();
	virtual void EndArray();

protected:
	virtual void PushState();
	virtual void PopState();

private:
	struct ParseState
	{
		ParseState();

		std::string currentKey;
		std::vector<const json::Value*> currentValue;
		size_t currentIndex;
	};

	const json::Value& ReadValue();

	static const std::string gNoKey;

	json::Value mRootNode;
	std::vector<ParseState> mStateStack;
};

class SceneJsonSerializer : public SceneSerializer
{
public:
	SceneJsonSerializer(ComponentFactory& componentFactory, std::ostream& output);
	~SceneJsonSerializer(void);
	
	virtual void WriteNull();
	virtual void WriteFloat(float value);
	virtual void WriteInt(int integer);
	virtual void WriteString(const std::string& value);
	virtual void WriteBool(bool value);
	virtual void WriteVector3(const krono::Vector3f& value);
	virtual void WriteQuaternion(const krono::Quaternionf& value);

	virtual void BeginObject();
	virtual void WriteKey(const std::string& name);
	virtual void EndObject();

	virtual void BeginArray();
	virtual void EndArray();
protected:
	virtual void Finish();
private:
	static const std::string gNoKey;

	json::Value& InsertValue(const json::Value& value);

	json::Value mRootNode;
	std::vector<json::Value*> mCurrentValue;
	std::string mCurrentKey;
};

}