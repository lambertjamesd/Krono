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

void LuaFunctionBinding::AddKGEBinPreload(LuaContext& context)
{
	lua_State* state = context.GetState();
	lua_getglobal(state, "package");
	lua_getfield(state, -1, "preload");
	lua_pushliteral(state, KGE_CPP_MODUlE_NAME);
	lua_pushcfunction(state, BuildKGEModule);
	lua_rawset(state, -3);

	lua_pop(state, 2);
}

int LuaFunctionBinding::BuildKGEModule(lua_State* state)
{
	int top = lua_gettop(state);

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

	int endtop = lua_gettop(state);

	return 1;
}

void LuaFunctionBinding::BuildGameObjectClass(LuaContext& context)
{
	context.BeginUserDataClass<GameObject>(LUA_GAME_OBJECT_NAME, LuaContext::NoBaseClass);
	context.PushClassMethod(&GameObject::GetComponent<Transform>);
	context.SetGetter("transform");
	context.PushClassMethod(&GameObject::Destroy);
	context.SetMethod("Destroy");

	context.EndClass();
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

	context.PushClassMethod(&Transform::GetLocalPosition);
	context.SetGetter("localPosition");
	context.PushClassMethod(&Transform::SetLocalPosition);
	context.SetSetter("localPosition");

	context.PushClassMethod(&Transform::GetLocalOrientation);
	context.SetGetter("localRotation");
	context.PushClassMethod(&Transform::SetLocalOrientation);
	context.SetSetter("localRotation");

	context.PushClassMethod(&Transform::GetLocalScale);
	context.SetGetter("localScale");
	context.PushClassMethod(&Transform::SetLocalScale);
	context.SetSetter("localScale");
	
	context.PushClassMethod(&Transform::GetParent);
	context.SetGetter("parent");
	context.PushClassMethod(&Transform::SetParent);
	context.SetSetter("parent");

	context.EndClass();
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

	lua_pushboolean(context.GetState(), true);
	context.SetMethod("isWeak");

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
	
	lua_pushboolean(context.GetState(), false);
	context.SetMethod("isWeak");

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