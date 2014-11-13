#pragma once

#include "GameObject/Behavior.h"
#include "LuaScript.h"
#include "LuaContext.h"

namespace kge
{

class LuaBehavior : public Behavior
{
public:
	typedef std::shared_ptr<LuaBehavior> Ptr;

	LuaBehavior(GameObject& parentGameObject);
	~LuaBehavior(void);

	virtual void Update(float deltaTime);

	const std::string& GetLuaClassName() const;
	void SetLuaClassName(const std::string& value);
	
	virtual void Serialize(SceneSerializer& serializer);
	virtual void Deserialize(SceneDeserializer& deserializer);
private:

	LuaContext& mContext;
	std::string mLuaClassName;
	size_t mObjectID;

	bool mHasUpdate;
};

}