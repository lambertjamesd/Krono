#include "LuaContext.h"
#include <cassert>
#include <memory>

#include "LuaFunctionBinding.h"

using namespace std;
using namespace krono;

namespace kge
{

LuaErrorException::LuaErrorException(const std::string& message) :
	Exception(message)
{

}

LuaMethodNotFoundException::LuaMethodNotFoundException(const std::string& message) :
	Exception(message)
{

}

LuaWeakRefUserData::LuaWeakRefUserData(const Object::Ref& ref, void* ptr) :
	ref(ref),
	ptr(ptr)
{

}

LuaWeakRefUserData::~LuaWeakRefUserData()
{

}

LuaContext::LuaContext(void) :
	mCurrentObjectID(0x100), // don't start at 1 to make lua tables use the hash index,
	mDebuggerLoaded(false),
	mDebuggerConnected(false),
	mDebuggerEnabled(false)
{
	mLuaState = luaL_newstate();
	luaL_openlibs(mLuaState);
	LuaFunctionBinding::AddKGEBinPreload(*this);
	
	// save context
	lua_pushlightuserdata(mLuaState, this);
	lua_setglobal(mLuaState, CONTEXT_KEY);

	// add engine lua pakcage path
	lua_getglobal(mLuaState, "package");
	lua_getfield(mLuaState, -1, "path");
	lua_pushliteral(mLuaState, LUA_MODULE_ENGINE_PATH);
	lua_concat(mLuaState, 2);
	lua_setfield(mLuaState, -2, "path");
	lua_pop(mLuaState, 1);

	Require("jit");
	
	// import debugger
	if (Require("mobdebug"))
	{
		lua_getfield(mLuaState, LUA_REGISTRYINDEX, "mobdebug");
		lua_getfield(mLuaState, -1, "coro");
		lua_call(mLuaState, 0, 0);
		lua_pop(mLuaState, 1);

		mDebuggerLoaded = true;
		ConnectDebugger();

		// the debugger doesn't actually load until some code is run
		// so run a simple program to check to see it loads property
		luaL_loadstring(mLuaState, "return nil");
		int callResult = lua_pcall(mLuaState, 0, 0, 0);

		if (callResult != 0)
		{
			std::cerr << "Could not connect debugger: " << lua_tostring(mLuaState, -1)  << std::endl;
			mDebuggerConnected = false;
		}
	}

	Require("kge");
}


LuaContext::~LuaContext(void)
{
	lua_close(mLuaState);
}

bool LuaContext::Require(const char* moduleName)
{
	// import module
	lua_getglobal(mLuaState, "require");
	lua_pushstring(mLuaState, moduleName);
	int requireResult = lua_pcall(mLuaState, 1, 1, 0);

	if (requireResult != 0)
	{
		// import failed
		std::cerr << lua_tostring(mLuaState, -1) << std::endl;
		lua_pop(mLuaState, 1);
		return false;
	}
	else
	{
		lua_setfield(mLuaState, LUA_REGISTRYINDEX, moduleName);
		return true;
	}
}

lua_State* LuaContext::GetState()
{
	return mLuaState;
}

void LuaContext::BeginUserDataClass(const std::string& name, const std::string& baseClass, const std::type_info& classType)
{
	BeginClass(name, baseClass);
	mTypeMapping[classType.hash_code()] = name;
}

void LuaContext::BeginClass(const std::string& name, const std::string& baseClass)
{
	lua_getfield(mLuaState, -1, baseClass.c_str());
	// stack = baseclass
	if (lua_istable(mLuaState, -1))
	{
		DeepCopy(-1);
	}
	else
	{
		lua_newtable(mLuaState);
	
		lua_pushliteral(mLuaState, "__index");
		lua_pushcfunction(mLuaState, LuaObjectIndexMethod);
		lua_rawset(mLuaState, -3);

		lua_pushliteral(mLuaState, "__newindex");
		lua_pushcfunction(mLuaState, LuaObjectNewIndexMethod);
		lua_rawset(mLuaState, -3);
	}
		
	// stack = baseclass | class

	// set the class name
	lua_pushliteral(mLuaState, "className");
	lua_pushstring(mLuaState, name.c_str());
	lua_rawset(mLuaState, -3);

	lua_pushliteral(mLuaState, "base");
	// copy base class
	lua_pushvalue(mLuaState, -3);
	// set base class
	lua_rawset(mLuaState, -3);

	// remove base class
	lua_remove(mLuaState, -2);
	
	// save class to module
	// stack = class
	lua_pushvalue(mLuaState, -1);
	lua_setfield(mLuaState, -3, name.c_str());
}

void LuaContext::AddMethod(const std::string& name, lua_CFunction method)
{
	lua_pushcfunction(mLuaState, method);
	SetMethod(name);
}

void LuaContext::AddProperty(const std::string& name, lua_CFunction getter, lua_CFunction setter)
{
	if (getter != NULL)
	{
		lua_pushcfunction(mLuaState, getter);
		SetGetter(name);
	}

	if (setter != NULL)
	{
		lua_pushcfunction(mLuaState, setter);
		SetSetter(name);
	}
}

void LuaContext::SetMethod(const std::string& name)
{
	lua_setfield(mLuaState, -2, name.c_str());
}

void LuaContext::SetGetter(const std::string& name)
{
	SetPropertyField(name, KGE_GETTER_KEY);
}

void LuaContext::SetSetter(const std::string& name)
{
	SetPropertyField(name, KGE_SETTER_KEY);
}

void LuaContext::SetPropertyField(const std::string& name, const char* fieldName)
{
	// stack = class | getter
	lua_getfield(mLuaState, -2, name.c_str());

	if (lua_isnil(mLuaState, -1))
	{
		lua_pop(mLuaState, 1);
		lua_newtable(mLuaState);
		lua_pushvalue(mLuaState, -1);

		// stack = class | getter | propertyTable | propertyTable
		lua_setfield(mLuaState, -4, name.c_str());
	}
	else if (!lua_istable(mLuaState, -1))
	{
		throw Exception("Cannot add getter or setter to non table value");
	}

	// stack = class | getter | propertyTable
	lua_pushboolean(mLuaState, true);
	lua_setfield(mLuaState, -2, KGE_IS_PROPERTY_KEY);

	lua_insert(mLuaState, -2);
	
	// stack = class | propertyTable | getter
	lua_setfield(mLuaState, -2, fieldName);
	lua_pop(mLuaState, 1);
}

void LuaContext::AddMetaMethod(const std::string& name, lua_CFunction method)
{
	lua_pushstring(mLuaState, name.c_str());
	lua_pushcfunction(mLuaState, method);
	lua_rawset(mLuaState, -3);
}

int LuaContext::LuaCallFunctionBase(lua_State* state)
{
	LuaFunctionBase* fn = (LuaFunctionBase*)lua_touserdata(state, lua_upvalueindex(1));
	return fn->Apply(state);
}

void LuaContext::EndClass()
{
	lua_pop(mLuaState, 1);
}

int LuaContext::PositiveIndex(lua_State* state, int index)
{
	if (index < 0 && index != LUA_GLOBALSINDEX && index != LUA_REGISTRYINDEX)
	{
		return lua_gettop(state) + index + 1;
	}
	else
	{
		return index;
	}
}

void LuaContext::DeepCopy(int index)
{
	index = PositiveIndex(mLuaState, index);

	if (lua_istable(mLuaState, index))
	{
		lua_newtable(mLuaState);

		lua_pushnil(mLuaState);

		// stack = source ... | table copy | nil

		// foreach keypair in the table to copy
		while (lua_next(mLuaState, index) != 0)
		{	
			// copy the key
			lua_pushvalue(mLuaState, -2);
			
			// stack = source ... | table copy | key | value | key

			// copy the value
			DeepCopy(-2);

			// stack = source ... | table copy | key | value | key | value copy

			// asign the copy in the table copy
			lua_settable(mLuaState, -5);
			// remove value for next iteration
			lua_pop(mLuaState, 1);
		}
	}
	else
	{
		lua_pushvalue(mLuaState, index);
	}
}

void LuaContext::ShallowCopy(int index)
{
	index = PositiveIndex(mLuaState, index);

	if (lua_istable(mLuaState, index))
	{
		lua_newtable(mLuaState);

		lua_pushnil(mLuaState);

		// stack = source ... | table copy | nil

		// foreach keypair in the table to copy
		while (lua_next(mLuaState, index) != 0)
		{
			// copy the key
			lua_pushvalue(mLuaState, -2);
			
			// stack = source ... | table copy | key | value | key

			// copy the value
			lua_pushvalue(mLuaState, -2);

			// stack = source ... | table copy | key | value | key | value copy

			// asign the copy in the table copy
			lua_settable(mLuaState, -5);
			// remove value for next iteration
			lua_pop(mLuaState, 1);
		}
	}
	else
	{
		lua_pushvalue(mLuaState, index);
	}
}

void LuaContext::PushVector3(lua_State* state, const krono::Vector3f& vector)
{
	lua_createtable(state, 3, 0);

	GetKGEField(state, LUA_VECTOR3_NAME);
	lua_setmetatable(state, -2);

	lua_pushinteger(state, 1);
	lua_pushnumber(state, vector.x);
	lua_rawset(state, -3);
	
	lua_pushinteger(state, 2);
	lua_pushnumber(state, vector.y);
	lua_rawset(state, -3);
	
	lua_pushinteger(state, 3);
	lua_pushnumber(state, vector.z);
	lua_rawset(state, -3);
}

void LuaContext::PushQuaternion(lua_State* state, const krono::Quaternionf& quaternion)
{
	lua_createtable(state, 4, 0);

	GetKGEField(state, LUA_QUATERNION_NAME);
	lua_setmetatable(state, -2);

	lua_pushinteger(state, 1);
	lua_pushnumber(state, quaternion.x);
	lua_rawset(state, -3);
	
	lua_pushinteger(state, 2);
	lua_pushnumber(state, quaternion.y);
	lua_rawset(state, -3);
	
	lua_pushinteger(state, 3);
	lua_pushnumber(state, quaternion.z);
	lua_rawset(state, -3);
	
	lua_pushinteger(state, 4);
	lua_pushnumber(state, quaternion.w);
	lua_rawset(state, -3);
}


krono::Vector3f LuaContext::ToVector3(lua_State* state, int index)
{
	index = PositiveIndex(state, index);
	
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

krono::Quaternionf LuaContext::ToQuaternion(lua_State* state, int index)
{
	index = PositiveIndex(state, index);
	
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

size_t LuaContext::PushPointer(const krono::Object::Ptr& value, const std::string& luaClassName)
{
	auto existingObject = mPointerIDMapping.find(value.get());

	if (existingObject == mPointerIDMapping.end())
	{
		lua_newtable(mLuaState);
		int tableIndex = PositiveIndex(mLuaState, -1);

		GetKGEField(mLuaState, luaClassName.c_str());
		lua_setmetatable(mLuaState, tableIndex);

		lua_pushliteral(mLuaState, OBJECT_REFERENCE_KEY);
		PushPointer(mLuaState, value);
		lua_rawset(mLuaState, tableIndex);

		mPointerIDMapping[value.get()] = mCurrentObjectID;
			
		// save the object
		GetKGEField(mLuaState, ALL_POINTERS_KEY);
		lua_pushinteger(mLuaState, mCurrentObjectID);
		lua_pushvalue(mLuaState, tableIndex);
		lua_rawset(mLuaState, -3);
		lua_pop(mLuaState, 1);

		size_t result = mCurrentObjectID;
		++mCurrentObjectID;
		return result;
	}
	else
	{
		// the object already exists, just load it
		PushExistingPointer(existingObject->second);
		return existingObject->second;
	}
}

void LuaContext::PushExistingPointer(size_t id)
{
	GetKGEField(mLuaState, ALL_POINTERS_KEY);
	lua_pushinteger(mLuaState, id);
	lua_rawget(mLuaState, -2);
	lua_remove(mLuaState, -2);
}

bool LuaContext::IsWeakPointer(lua_State* state, int index)
{
	assert(lua_istable(state, index));
	index = PositiveIndex(state, index);
	lua_pushliteral(state, OBJECT_REFERENCE_KEY);
	lua_gettable(state, index);
	lua_getfield(state, -1, "isWeak");
	bool result = lua_toboolean(state, -1) != 0;
	lua_pop(state, 2);
	return result;
}

krono::Object::Ptr LuaContext::GetPointer(lua_State* state, int index)
{
	assert(lua_istable(state, index));
	index = PositiveIndex(state, index);

	lua_pushliteral(state, OBJECT_REFERENCE_KEY);
	lua_gettable(state, index);
	Object::Ptr result = ToPointer(state, -1);
	lua_pop(state, 1);
	
	// ensure that the object is saved in the strong pointer tables
	LuaContext *context = ContextFromState(state);
	GetKGEField(state, ALL_POINTERS_KEY);
	lua_pushinteger(state, context->mPointerIDMapping[result.get()]);
	lua_pushvalue(state, index);
	lua_rawset(state, -3);
	lua_pop(state, 1);

	return result;
}

void LuaContext::PushPointer(lua_State* state, const krono::Object::Ptr& value)
{
	void* userData = lua_newuserdata(state, sizeof(Object::Ptr));
	new (userData) Object::Ptr(value);

	GetKGEField(state, LUA_SHARED_PTR_NAME);
	lua_setmetatable(state, -2);
}

krono::Object::Ptr LuaContext::ToPointer(lua_State* state, int index)
{
	return *(Object::Ptr*)lua_touserdata(state, -1);
}

size_t LuaContext::PushReference(const krono::Object::Ref& value)
{
	// set the meta table
	auto luaClassName = mTypeMapping.find(typeid(*value.lock().get()).hash_code());

	if (luaClassName != mTypeMapping.end())
	{
		return PushReference(value, luaClassName->second);
	}
	else
	{
		return PushReference(value, "");
	}
}

size_t LuaContext::PushReference(const Object::Ref& value, const std::string& luaClassName)
{
	assert(!value.expired());

	auto existingObject = mPointerIDMapping.find(value.lock().get());

	if (existingObject == mPointerIDMapping.end())
	{
		lua_newtable(mLuaState);
		int tableIndex = PositiveIndex(mLuaState, -1);

		GetKGEField(mLuaState, luaClassName.c_str());
		lua_setmetatable(mLuaState, tableIndex);

		lua_pushliteral(mLuaState, OBJECT_REFERENCE_KEY);
		PushWeakRef(mLuaState, value);
		lua_rawset(mLuaState, tableIndex);

		mPointerIDMapping[value.lock().get()] = mCurrentObjectID;
			
		// save the object
		GetKGEField(mLuaState, ALL_REFERENCES_KEY);
		lua_pushinteger(mLuaState, mCurrentObjectID);
		lua_pushvalue(mLuaState, tableIndex);
		lua_rawset(mLuaState, -3);
		lua_pop(mLuaState, 1);

		size_t result = mCurrentObjectID;
		++mCurrentObjectID;
		return result;
	}
	else
	{
		// the object already exists, just load it
		PushExistingReference(existingObject->second);
		return existingObject->second;
	}
}

void LuaContext::PushExistingReference(size_t id)
{
	GetKGEField(mLuaState, ALL_REFERENCES_KEY);
	lua_pushinteger(mLuaState, id);
	lua_rawget(mLuaState, -2);
	lua_remove(mLuaState, -2);
}

Object::Ref LuaContext::GetObjectReference(lua_State* state, int tableIndex)
{
	assert(lua_istable(state, tableIndex));
	tableIndex = PositiveIndex(state, tableIndex);
	lua_pushliteral(state, OBJECT_REFERENCE_KEY);
	lua_gettable(state, tableIndex);
	Object::Ref result = ToWeakRef(state, -1);
	lua_pop(state, 1);
	return result;
}

void LuaContext::PushWeakRef(lua_State* state, const Object::Ref& value)
{
	void* userData = lua_newuserdata(state, sizeof(LuaWeakRefUserData));
	new (userData) LuaWeakRefUserData(value, value.lock().get());

	GetKGEField(state, LUA_WEAK_REF_NAME);
	lua_setmetatable(state, -2);
}

Object::Ref LuaContext::ToWeakRef(lua_State* state, int index)
{
	return ((LuaWeakRefUserData*)lua_touserdata(state, -1))->ref;
}

LuaContext* LuaContext::ContextFromState(lua_State* state)
{
	lua_getglobal(state, CONTEXT_KEY);
	LuaContext* result = (LuaContext*)lua_touserdata(state, -1);
	lua_pop(state, 1);
	return result;
}

void LuaContext::AddBehaviorScript(const LuaScript::Ptr& script)
{
	AddBehaviorScript(script, FileHelper::RemoveExtension(FileHelper::LastPathElement(script->GetSource())));
}

void LuaContext::AddBehaviorScript(const LuaScript::Ptr& script, const std::string& behaviorName)
{
	try
	{
		if (script->GetSource().length() > 0)
		{
			GetKGEField(mLuaState, "MapClassToPath");
			CallStaticMethod<void>(NULL, behaviorName, script->GetSource());
		}
		else
		{
			GetKGEField(mLuaState, "MapClassToSource");
			CallStaticMethod<void>(NULL, behaviorName, script->GetSourceCode());
		}
	}
	catch (LuaErrorException& error)
	{
		std::cerr << "Error adding behavior: " << error.what() << std::endl;
	}
}

bool LuaContext::LoadClassTable(const std::string& value)
{
	try
	{
		GetKGEField(mLuaState, "RequireClass");
		CallStaticMethod<LuaCppConvert::ReturnToStack>(NULL, value);
		return true;
	}
	catch (LuaErrorException& error)
	{
		std::cerr << "Error loading class: " << error.what() << std::endl;
		return false;
	}
}

int LuaContext::WeakReferenceGC(lua_State* state)
{
	assert(lua_gettop(state) == 1 && lua_isuserdata(state, -1));
	LuaWeakRefUserData* weakRef = (LuaWeakRefUserData*)lua_touserdata(state, -1);

	ContextFromState(state)->RemovePointerIndex(weakRef->ptr);

	weakRef->~LuaWeakRefUserData();
	lua_pop(state, 1);
	return 0;
}

int LuaContext::SharedPtrGC(lua_State* state)
{
	assert(lua_gettop(state) == 1 && lua_isuserdata(state, -1));
	Object::Ptr* sharedPtr = (Object::Ptr*)lua_touserdata(state, -1);

	ContextFromState(state)->RemovePointerIndex(sharedPtr->get());

	sharedPtr->~shared_ptr();
	return 0;
}

void LuaContext::GetKGEField(lua_State* state, const char* fieldName)
{
	lua_getfield(state, LUA_REGISTRYINDEX, KGE_CPP_MODUlE_NAME);
	lua_getfield(state, -1, fieldName);
	lua_remove(state, -2);
}

void LuaContext::ConnectDebugger(const char* host, unsigned short port)
{
	if (mDebuggerLoaded)
	{
		lua_getfield(mLuaState, LUA_REGISTRYINDEX, "mobdebug");
	
		lua_getfield(mLuaState, -1, "start");
		lua_pushstring(mLuaState, host);
		lua_pushnumber(mLuaState, port);
		int result = lua_pcall(mLuaState, 2, 0, 0);

		if (result != 0)
		{
			std::cerr << lua_tostring(mLuaState, -1) << std::endl;
			lua_pop(mLuaState, 1);
		}
		else
		{
			mDebuggerConnected = true;
		}

		lua_pop(mLuaState, 1);
	}
}

void LuaContext::PauseDebugger()
{
	if (mDebuggerLoaded && mDebuggerConnected)
	{
		lua_getfield(mLuaState, LUA_REGISTRYINDEX, "mobdebug");
	
		lua_getfield(mLuaState, -1, "off");
		lua_call(mLuaState, 0, 0);

		lua_pop(mLuaState, 1);

		mDebuggerConnected = true;
	}
}

void LuaContext::ResumeDebugger()
{
	if (mDebuggerLoaded && mDebuggerConnected)
	{
		lua_getfield(mLuaState, LUA_REGISTRYINDEX, "mobdebug");
	
		lua_getfield(mLuaState, -1, "on");
		lua_call(mLuaState, 0, 0);

		lua_pop(mLuaState, 1);

		mDebuggerConnected = true;
	}
}

void LuaContext::RemovePointerIndex(void* pointer)
{
	mPointerIDMapping.erase(pointer);
}

bool LuaContext::IsProperty(lua_State* state, int index)
{
	if (!lua_istable(state, index))
	{
		return false;
	}

	lua_getfield(state, index, KGE_IS_PROPERTY_KEY);
	bool result = lua_toboolean(state, -1) != 0;
	lua_pop(state, 1);
	return result;
}

bool LuaContext::PushPropertyFunction(lua_State* state, int index, const char* functionName)
{
	if (!IsProperty(state, index))
	{
		return false;
	}
	else
	{
		lua_getfield(state, index, functionName);

		if (lua_isfunction(state, -1) != 0)
		{
			return true;
		}
		else
		{
			lua_pop(state, 1);
			return false;
		}
	}
}

int LuaContext::LuaObjectIndexMethod(lua_State* state)
{
	// stack = table | key
	if (lua_getmetatable(state, -2))
	{
		lua_insert(state, -2);

		// stack = table | metatable | key

		lua_rawget(state, -2);
		
		// stack = table | metatable | result

		if (PushPropertyFunction(state, -1, KGE_GETTER_KEY))
		{
			int startStackSize = lua_gettop(state) - 1;

			// get table copy
			lua_pushvalue(state, -4);
			
			int callResult = lua_pcall(state, 1, LUA_MULTRET, 0);

			int resultCount = lua_gettop(state) - startStackSize;

			return resultCount;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		lua_pop(state, 2);
		lua_pushliteral(state, "No metatable set on object");
		lua_error(state);
		return 0;
	}
}

int LuaContext::LuaObjectNewIndexMethod(lua_State* state)
{
	// stack = table | key | value
	if (lua_getmetatable(state, -3))
	{
		// stack = table | key | value | metatable

		lua_pushvalue(state, -3);
		lua_rawget(state, -2);
		
		// stack = table | key | value | metatable | property
		if (IsProperty(state, -1))
		{
			if (PushPropertyFunction(state, -1, KGE_SETTER_KEY))
			{
				// stack = table | key | value | metatable | property | getter

				// move the setter into the top of the stack
				lua_insert(state, -6);
				// remove the metatable and property
				lua_pop(state, 2);
				// remove the key
				lua_remove(state, -2);

				// stack = setter | table | value

				lua_call(state, 2, 0);
				return 0;
			}
			else
			{
				lua_pushliteral(state, "No setter defined for property");
				lua_error(state);
				return 0;
			}
		}
		else
		{
			// remove property table and metatable
			lua_pop(state, 2);

			// just rawset the value
			lua_rawset(state, -3);

			return 0;
		}
	}
	else
	{
		lua_pop(state, 3);
		lua_pushliteral(state, "No metatable set on object");
		lua_error(state);
		return 0;
	}
}

const char* LuaContext::NoBaseClass = "";

}