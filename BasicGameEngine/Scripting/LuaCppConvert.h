#pragma once

#include "Lua.h"

#include "GameObject/Transform.h"

namespace kge
{	

namespace LuaCppConvert
{
	struct Pass {
		template<typename ...T> Pass(T...) {}
	};

	template <typename T>
	struct ReturnTypeCount
	{
		static const size_t Value = 1;
	};

	template<>
	struct ReturnTypeCount<void>
	{
		static const size_t Value = 0;
	};
	
	template <typename T>
	T ConvertToCpp(lua_State* state, int index);

	template <>
	Transform::Ref ConvertToCpp(lua_State* state, int index);
	
	template <>
	krono::Vector3f ConvertToCpp(lua_State* state, int index);
	
	template <>
	krono::Quaternionf ConvertToCpp(lua_State* state, int index);

	void ConvertToLua(lua_State* state, const float& value);
	void ConvertToLua(lua_State* state, const double& value);
	void ConvertToLua(lua_State* state, const int& value);
	void ConvertToLua(lua_State* state, const krono::Quaternionf& value);
	void ConvertToLua(lua_State* state, const krono::Vector3f& value);
	void ConvertToLua(lua_State* state, const krono::Object::Ref& ptr);

	template <typename T>
	void ConvertToLua(lua_State* state, const std::shared_ptr<T>& ptr)
	{
		LuaContext* context = LuaContext::ContextFromState(state);
		context->PushPointer<T>(ptr);
	}

	template <typename T>
	void ConvertToLua(lua_State* state, const std::weak_ptr<T>& ptr)
	{
		LuaContext* context = LuaContext::ContextFromState(state);
		context->PushReference<T>(ptr);
	}

	krono::Object* GetObjectPointer(lua_State* state, int index);

	template <typename T>
	struct ConstRemove
	{
		typedef T ConstFreeType;
	};

	template <>
	struct ConstRemove<const Transform::Ref&>
	{
		typedef Transform::Ref ConstFreeType;
	};
}

}