#pragma once

#include "LuaContext.h"
#include "GameObject/Transform.h"

#define LUA_WEAK_REF_NAME		"WeakRef"
#define LUA_SHARED_PTR_NAME		"SharedPtr"
#define LUA_VECTOR3_NAME		"Vector3"
#define LUA_QUATERNION_NAME		"Quaternion"

#define LUA_GAME_OBJECT_NAME	"GameObject"

#define LUA_COMPONENT_NAME		"Component"
#define LUA_TRANSFORM_NAME		"Transform"

#define LUA_BEHAVIOR_NAME		"Behavior"
#define LUA_LUA_BEHAVIOR_NAME	"LuaBehavior"

#define LUA_CREATE_GC_Canary	"CreateGCCanary"

namespace kge
{

class LuaFunctionBinding
{
public:

	static void BuildContext(LuaContext& context);

private:
	static int BuildKGEModule(lua_State* state);

	static void BuildGameObjectClass(LuaContext& context);

	static void BuildComponentClass(LuaContext& context);
	static int ComponentGameObjectGetter(lua_State* state);

	static void BuildTransformClass(LuaContext& context);
	
	static void BuildBehaviorClass(LuaContext& context);

	static void BuildWeakReferenceClass(LuaContext& context);
	static int WeakReferenceIsExpired(lua_State* state);
	
	static void BuildSharedPtrClass(LuaContext& context);
	static int SharedPtrIsExpired(lua_State* state);
	static int SharedPtrIsUnique(lua_State* state);
	
	static int LuaCreateGCCanary(lua_State* state);
};

}