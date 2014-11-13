#include "LuaBehavior.h"
#include "GameObject/GameObject.h"
#include "Scene/Game.h"
#include "Serialization/SceneSerializer.h"

namespace kge
{

LuaBehavior::LuaBehavior(GameObject& parentGameObject) :
	Behavior(parentGameObject),
	mContext(parentGameObject.GetScene().GetGame().GetLuaContext()),
	mObjectID(~0),
	mHasUpdate(false)
{
	
}


LuaBehavior::~LuaBehavior(void)
{
	std::cout << "Here I am" << std::endl;
}

void LuaBehavior::Update(float deltaTime)
{
	if (mObjectID != ~0 && mHasUpdate)
	{
		mContext.PushExistingReference(mObjectID);
		lua_State* state = mContext.GetState();

		try
		{
			mContext.CallMethod<void>("Update", deltaTime);
		}
		catch (LuaErrorException& luaError)
		{
			std::cerr << luaError.what() << std::endl;
		}

		// pop the script object
		lua_pop(state, 1);
	}
}

const std::string& LuaBehavior::GetLuaClassName() const
{
	return mLuaClassName;
}

void LuaBehavior::Serialize(SceneSerializer& serializer)
{
	serializer.WriteString("className", mLuaClassName);
}

void LuaBehavior::Deserialize(SceneDeserializer& deserializer)
{
	SetLuaClassName(deserializer.ReadString("className", "LuaBehavior"));
}

void LuaBehavior::SetLuaClassName(const std::string& value)
{
	if (mLuaClassName != value)
	{
		mLuaClassName = value;

		mObjectID = mContext.PushReference<LuaBehavior>(GetWeakPointer());
		lua_State* state = mContext.GetState();
		LuaContext::GetKGEField(state, KGE_BEHAVIORS_NAME);
		lua_getfield(state, -1, mLuaClassName.c_str());
		lua_remove(state, -2);
		lua_setmetatable(state, -2);

		lua_pushliteral(state, "Update");
		lua_gettable(state, -2);

		mHasUpdate = lua_isfunction(state, -1);

		lua_pop(state, 2);
	}
}

}