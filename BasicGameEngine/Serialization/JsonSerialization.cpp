#include "JsonSerialization.h"
#include <cassert>

using namespace std;
using namespace krono;

namespace kge
{

SceneJsonDeserializer::SceneJsonDeserializer(ComponentFactory& componentFactory, krono::ResourceManager& resourceManager, std::istream& input) :
	SceneDeserializer(componentFactory, resourceManager)
{
	string jsonString(std::istreambuf_iterator<char>(input), (std::istreambuf_iterator<char>()));
	
	mRootNode = json::Deserialize(jsonString);

	mStateStack.push_back(ParseState());
}

SceneJsonDeserializer::~SceneJsonDeserializer()
{

}

SceneJsonDeserializer::ParseState::ParseState() :
	currentKey(SceneJsonDeserializer::gNoKey),
	currentValue(),
	currentIndex(0)
{

}

float SceneJsonDeserializer::ReadFloat(float defaultValue)
{
	return ReadValue().ToFloat(defaultValue);
}

int SceneJsonDeserializer::ReadInt(int defaultValue)
{
	return ReadValue().ToInt(defaultValue);
}

std::string SceneJsonDeserializer::ReadString(const std::string& defaultValue)
{
	return ReadValue().ToString(defaultValue);
}

bool SceneJsonDeserializer::ReadBool(bool defaultValue)
{
	return ReadValue().ToBool(defaultValue);
}

krono::Vector3f SceneJsonDeserializer::ReadVector3(const krono::Vector3f& defaultValue)
{
	const json::Value& result = ReadValue();
	return Vector3f(result[(size_t)0].ToFloat(0.0f), result[(size_t)1].ToFloat(0.0f), result[(size_t)2].ToFloat(0.0f));
}

krono::Quaternionf SceneJsonDeserializer::ReadQuaternion(const krono::Quaternionf& defaultValue)
{
	const json::Value& result = ReadValue();
	return Quaternionf(result[(size_t)0].ToFloat(0.0f), result[(size_t)1].ToFloat(0.0f), result[(size_t)2].ToFloat(0.0f), result[(size_t)3].ToFloat(0.0f));
}

void SceneJsonDeserializer::BeginObject()
{
	ParseState& state = mStateStack.back();

	const json::Value& objectValue = state.currentValue.size() == 0 ? mRootNode : ReadValue();

	if (objectValue.GetType() == json::ObjectVal)
	{
		state.currentValue.push_back(&objectValue);
	}
	else
	{
		state.currentValue.push_back(NULL);
	}
}

void SceneJsonDeserializer::ReadKey(const std::string& key)
{
	ParseState& state = mStateStack.back();
	assert(state.currentKey == gNoKey);
	state.currentKey = key;
}

void SceneJsonDeserializer::EndObject()
{
	mStateStack.back().currentValue.pop_back();
}

size_t SceneJsonDeserializer::BeginArray()
{
	const json::Value& objectValue = mStateStack.back().currentValue.size() == 0 ? mRootNode : ReadValue();
	
	PushState();
	ParseState& state = mStateStack.back();
	state.currentIndex =  0;

	if (objectValue.GetType() == json::ArrayVal)
	{
		state.currentValue.push_back(&objectValue);
		return objectValue.size();
	}
	else
	{
		state.currentValue.push_back(NULL);
		return 0;
	}
}

void SceneJsonDeserializer::EndArray()
{
	mStateStack.back().currentValue.pop_back();
	PopState();
}

void SceneJsonDeserializer::PushState()
{
	mStateStack.push_back(mStateStack.back());
}

void SceneJsonDeserializer::PopState()
{
	mStateStack.pop_back();
}

const json::Value& SceneJsonDeserializer::ReadValue()
{
	ParseState& state = mStateStack.back();
	assert(state.currentValue.size() > 0);
	
	const json::Value* currentObject = state.currentValue.back();

	if (currentObject == NULL || currentObject->GetType() == json::NULLVal)
	{
		state.currentKey = gNoKey;
		return json::Value::Null;
	}
	else if (currentObject->GetType() == json::ArrayVal)
	{
		assert(state.currentKey == gNoKey);
		return (*currentObject)[state.currentIndex++];
	}
	else
	{
		assert(state.currentKey != gNoKey);
		const json::Value& result = (*currentObject)[state.currentKey];
		state.currentKey = gNoKey;
		return result;
	}
}

const std::string SceneJsonDeserializer::gNoKey = "@@__no_key__@@";
	
SceneJsonSerializer::SceneJsonSerializer(ComponentFactory& componentFactory, std::ostream& output) :
	SceneSerializer(componentFactory, output),
	mCurrentKey(gNoKey)
{

}

SceneJsonSerializer::~SceneJsonSerializer(void)
{

}
	
void SceneJsonSerializer::WriteNull()
{
	InsertValue(json::NULLVal);
}

void SceneJsonSerializer::WriteFloat(float value)
{
	InsertValue(json::Value(value));
}

void SceneJsonSerializer::WriteInt(int value)
{
	InsertValue(json::Value(value));
}

void SceneJsonSerializer::WriteString(const std::string& value)
{
	InsertValue(json::Value(value));
}

void SceneJsonSerializer::WriteBool(bool value)
{
	InsertValue(json::Value(value));
}

void SceneJsonSerializer::WriteVector3(const krono::Vector3f& value)
{
	json::Array result;
	result.push_back(json::Value(value.x));
	result.push_back(json::Value(value.y));
	result.push_back(json::Value(value.z));
	InsertValue(json::Value(result));
}

void SceneJsonSerializer::WriteQuaternion(const krono::Quaternionf& value)
{
	json::Array result;
	result.push_back(json::Value(value.w));
	result.push_back(json::Value(value.x));
	result.push_back(json::Value(value.y));
	result.push_back(json::Value(value.z));
	InsertValue(json::Value(result));
}

void SceneJsonSerializer::BeginObject()
{
	if (mCurrentValue.size() == 0)
	{
		mRootNode = json::Value(json::Object());
		mCurrentValue.push_back(&mRootNode);
	}
	else
	{
		mCurrentValue.push_back(&InsertValue(json::Object()));
	}
}

void SceneJsonSerializer::WriteKey(const std::string& name)
{
	assert(mCurrentKey == gNoKey);
	mCurrentKey = name;
}

void SceneJsonSerializer::EndObject()
{
	assert(mCurrentValue.size() > 0 && mCurrentValue.back()->GetType() == json::ObjectVal);
	mCurrentValue.pop_back();
}

void SceneJsonSerializer::BeginArray() 
{
	if (mCurrentValue.size() == 0)
	{
		mRootNode = json::Value(json::Array());
		mCurrentValue.push_back(&mRootNode);
	}
	else
	{
		mCurrentValue.push_back(&InsertValue(json::Array()));
	}
}

void SceneJsonSerializer::EndArray() 
{
	assert(mCurrentValue.size() > 0 && mCurrentValue.back()->GetType() == json::ArrayVal);
	mCurrentValue.pop_back();
}

void SceneJsonSerializer::Finish()
{
	mOutput << json::Serialize(mRootNode);
}

const std::string SceneJsonSerializer::gNoKey = "@@__no_key__@@";

json::Value& SceneJsonSerializer::InsertValue(const json::Value& value)
{
	assert(mCurrentValue.size() > 0);

	json::Value& currentValue = *mCurrentValue.back();

	if (mCurrentValue.back()->GetType() == json::ObjectVal)
	{
		assert(mCurrentKey != gNoKey);

		currentValue.ObjectValue()[mCurrentKey] = value;
		json::Value& result = currentValue.ObjectValue()[mCurrentKey];
		mCurrentKey = gNoKey;

		return result;
	}
	else
	{
		assert(mCurrentKey == gNoKey);

		size_t newIndex = currentValue.size();

		currentValue.ArrayValue().push_back(value);

		return currentValue[newIndex];
	}
}

}
