#include "LuaCppConvert.h"
#include "LuaContext.h"
#include "LuaFunctionBinding.h"

using namespace std;
using namespace krono;

namespace kge
{

namespace LuaCppConvert
{
	template <>
	Transform::Ref ConvertToCpp(lua_State* state, int index)
	{
		return LuaContext::GetReferencePtr<Transform>(state, index);
	}

	template <>
	Vector3f ConvertToCpp(lua_State* state, int index)
	{
		index = LuaContext::PositiveIndex(state, index);
	
		lua_pushinteger(state, 1);
		lua_gettable(state, index);
		float x = (float)lua_tonumber(state, -1);
		lua_pop(state, 1);
	
		lua_pushinteger(state, 2);
		lua_gettable(state, index);
		float y = (float)lua_tonumber(state, -1);
		lua_pop(state, 1);
	
		lua_pushinteger(state, 3);
		lua_gettable(state, index);
		float z = (float)lua_tonumber(state, -1);
		lua_pop(state, 1);

		return Vector3f(x, y, z);
	}

	template <>
	krono::Quaternionf ConvertToCpp(lua_State* state, int index)
	{
		index = LuaContext::PositiveIndex(state, index);
	
		lua_pushinteger(state, 1);
		lua_gettable(state, index);
		float x = (float)lua_tonumber(state, -1);
		lua_pop(state, 1);
	
		lua_pushinteger(state, 2);
		lua_gettable(state, index);
		float y = (float)lua_tonumber(state, -1);
		lua_pop(state, 1);
	
		lua_pushinteger(state, 3);
		lua_gettable(state, index);
		float z = (float)lua_tonumber(state, -1);
		lua_pop(state, 1);
	
		lua_pushinteger(state, 4);
		lua_gettable(state, index);
		float w = (float)lua_tonumber(state, -1);
		lua_pop(state, 1);

		return Quaternionf(w, x, y, z);
	}
	
	template <>
	ReturnToStack ConvertToCpp(lua_State* state, int index)
	{
		return ReturnToStack();
	}

	void ConvertToLua(lua_State* state, const float& value)
	{
		lua_pushnumber(state, value);
	}
	
	void ConvertToLua(lua_State* state, const double& value)
	{
		lua_pushnumber(state, value);
	}
	
	void ConvertToLua(lua_State* state, const int& value)
	{
		lua_pushinteger(state, value);
	}
	
	void ConvertToLua(lua_State* state, const krono::Quaternionf& value)
	{
		lua_createtable(state, 4, 0);

		LuaContext::GetKGEField(state, LUA_QUATERNION_NAME);
		lua_setmetatable(state, -2);

		lua_pushinteger(state, 1);
		lua_pushnumber(state, value.x);
		lua_rawset(state, -3);
	
		lua_pushinteger(state, 2);
		lua_pushnumber(state, value.y);
		lua_rawset(state, -3);
	
		lua_pushinteger(state, 3);
		lua_pushnumber(state, value.z);
		lua_rawset(state, -3);
	
		lua_pushinteger(state, 4);
		lua_pushnumber(state, value.w);
		lua_rawset(state, -3);
	}
	
	void ConvertToLua(lua_State* state, const krono::Vector3f& value)
	{
		lua_createtable(state, 3, 0);

		LuaContext::GetKGEField(state, LUA_VECTOR3_NAME);
		lua_setmetatable(state, -2);

		lua_pushinteger(state, 1);
		lua_pushnumber(state, value.x);
		lua_rawset(state, -3);
	
		lua_pushinteger(state, 2);
		lua_pushnumber(state, value.y);
		lua_rawset(state, -3);
	
		lua_pushinteger(state, 3);
		lua_pushnumber(state, value.z);
		lua_rawset(state, -3);
	}

	void ConvertToLua(lua_State* state, const krono::Object::Ref& ptr)
	{
		LuaContext* context = LuaContext::ContextFromState(state);
		context->PushReference(ptr);
	}
	
	void ConvertToLua(lua_State* state, const std::string& string)
	{
		lua_pushstring(state, string.c_str());
	}
	
	krono::Object* GetObjectPointer(lua_State* state, int index)
	{
		if (LuaContext::IsWeakPointer(state, index))
		{
			return LuaContext::GetObjectReference(state, index).lock().get();
		}
		else
		{
			return LuaContext::GetPointer(state, index).get();
		}
	}
}

}