#include "LuaFunctionBinding.h"
#include "LuaBehavior.h"
#include "GameObject/Transform.h"
#include "GameObject/GameObject.h"
#include "GameObject/Behavior.h"
#include <cassert>

using namespace krono;
using namespace std;

namespace kge
{

void LuaFunctionBinding::BuildContext(LuaContext& context)
{
	lua_State* state = context.GetState();
	lua_getglobal(state, "package");
	lua_getfield(state, -1, "preload");
	lua_pushliteral(state, KGE_CPP_MODUlE_NAME);
	lua_pushcfunction(state, BuildKGEModule);
	lua_rawset(state, -3);
}

int LuaFunctionBinding::BuildKGEModule(lua_State* state)
{
	lua_newtable(state);

	lua_newtable(state);
	lua_setfield(state, -2, ALL_REFERENCES_KEY);
	
	lua_newtable(state);
	lua_setfield(state, -2, ALL_POINTERS_KEY);

	lua_pushvalue(state, -1);
	lua_setfield(state, LUA_REGISTRYINDEX, KGE_CPP_MODUlE_NAME);

	LuaContext& context = *LuaContext::ContextFromState(state);

	BuildGameObjectClass(context);

	BuildComponentClass(context);
	BuildTransformClass(context);
	BuildBehaviorClass(context);
	
	BuildWeakReferenceClass(context);

	lua_pushcfunction(state, LuaCreateGCCanary);
	lua_setfield(state, -2, LUA_CREATE_GC_Canary);

	return 1;
}

void LuaFunctionBinding::BuildGameObjectClass(LuaContext& context)
{
	context.BeginUserDataClass<GameObject>(LUA_GAME_OBJECT_NAME, LuaContext::NoBaseClass);
	context.AddProperty("transform", GameObjectTransformGetter, NULL);
	context.AddMethod("Destroy", GameObjectDestroy);
	context.EndClass();
}

int LuaFunctionBinding::GameObjectTransformGetter(lua_State* state)
{
	LuaContext* context = LuaContext::ContextFromState(state);
	GameObject::Ptr gameObjectPointer = LuaContext::GetReferencePtr<GameObject>(state, -1);
	Object::Ref transformRef = gameObjectPointer->GetTransform();
	context->PushReference<Transform>(transformRef);
	return 1;
}

int LuaFunctionBinding::GameObjectDestroy(lua_State* state)
{
	LuaContext* context = LuaContext::ContextFromState(state);
	GameObject::Ptr gameObjectPointer = LuaContext::GetReferencePtr<GameObject>(state, -1);
	gameObjectPointer->Destroy();
	return 0;
}

void LuaFunctionBinding::BuildComponentClass(LuaContext& context)
{
	context.BeginUserDataClass<Component>(LUA_COMPONENT_NAME, LuaContext::NoBaseClass);
	context.AddProperty("gameObject", ComponentGameObjectGetter, NULL);
	context.EndClass();
}

int LuaFunctionBinding::ComponentGameObjectGetter(lua_State* state)
{
	LuaContext* context = LuaContext::ContextFromState(state);
	Component::Ptr componentPointer = LuaContext::GetReferencePtr<Component>(state, -1);
	lua_pop(state, 1);
	Object::Ref gameObjectRef = componentPointer->GetGameObject().GetWeakPointer();
	context->PushReference<GameObject>(gameObjectRef);
	return 1;
}

void LuaFunctionBinding::BuildTransformClass(LuaContext& context)
{
	context.BeginUserDataClass<Transform>(LUA_TRANSFORM_NAME, LUA_COMPONENT_NAME);
	context.AddProperty("localPosition", TransformLocalPositionGetter, TransformLocalPositionSetter);
	context.EndClass();
}

int LuaFunctionBinding::TransformLocalPositionGetter(lua_State* state)
{
	Transform::Ptr transformPointer = LuaContext::GetReferencePtr<Transform>(state, -1);
	lua_pop(state, 1);
	LuaContext::PushVector3(state, transformPointer->GetLocalPosition());
	return 1;
}

int LuaFunctionBinding::TransformLocalPositionSetter(lua_State* state)
{
	Transform::Ptr transformPointer = LuaContext::GetReferencePtr<Transform>(state, -2);
	transformPointer->SetLocalPosition(LuaContext::ToVector3(state, -1));
	lua_pop(state, 2);
	return 0;
}

void LuaFunctionBinding::BuildBehaviorClass(LuaContext& context)
{
	context.BeginUserDataClass<Behavior>(LUA_BEHAVIOR_NAME, LUA_COMPONENT_NAME);
	context.EndClass();
	context.BeginUserDataClass<LuaBehavior>(LUA_LUA_BEHAVIOR_NAME, LUA_BEHAVIOR_NAME);
	context.EndClass();
}

void LuaFunctionBinding::BuildWeakReferenceClass(LuaContext& context)
{
	context.BeginClass(LUA_WEAK_REF_NAME, LuaContext::NoBaseClass);
	context.AddMetaMethod("__gc", LuaContext::WeakReferenceGC);
	context.AddProperty("isExpired", WeakReferenceIsExpired, NULL);
	context.EndClass();
}

int LuaFunctionBinding::WeakReferenceIsExpired(lua_State* state)
{
	assert(lua_gettop(state) == 1 && lua_isuserdata(state, -1));
	LuaWeakRefUserData* weakRef = (LuaWeakRefUserData*)lua_touserdata(state, -1);
	lua_pop(state, 1);
	lua_pushboolean(state, weakRef->ref.expired());
	return 1;
}

void LuaFunctionBinding::BuildSharedPtrClass(LuaContext& context)
{
	context.BeginClass(LUA_SHARED_PTR_NAME, LuaContext::NoBaseClass);
	context.AddMetaMethod("__gc", LuaContext::SharedPtrGC);
	context.AddProperty("isExpired", SharedPtrIsExpired, NULL);
	context.AddProperty("isUnique", SharedPtrIsUnique, NULL);
	context.EndClass();
}

int LuaFunctionBinding::SharedPtrIsExpired(lua_State* state)
{
	lua_pushboolean(state, false);
	return 1;
}

int LuaFunctionBinding::SharedPtrIsUnique(lua_State* state)
{
	assert(lua_gettop(state) == 1 && lua_isuserdata(state, -1));
	Object::Ptr* sharedPtr = (Object::Ptr*)lua_touserdata(state, -1);
	lua_pushboolean(state, sharedPtr->unique());
	return 1;
}

int LuaFunctionBinding::LuaCreateGCCanary(lua_State* state)
{
	if (lua_gettop(state) == 0)
	{
		lua_pushnil(state);
	}

	lua_newuserdata(state, sizeof(int));
	lua_newtable(state);
	lua_pushliteral(state, "__gc");
	lua_pushvalue(state, 1);
	lua_rawset(state, -3);
	lua_setmetatable(state, -2);

	return 1;
}


}