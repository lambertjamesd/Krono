#include "LuaContext.h"
#include <cassert>
#include <memory>

#include "LuaFunctionBinding.h"

using namespace std;
using namespace krono;

namespace kge
{

LuaWeakRefUserData::LuaWeakRefUserData(const Object::Ref& ref, void* ptr) :
	ref(ref),
	ptr(ptr)
{

}

LuaWeakRefUserData::~LuaWeakRefUserData()
{

}

LuaContext::LuaContext(void) :
	mCurrentObjectID(0x100) // don't start at 1 to make lua tables use the hash index
{
	mLuaState = luaL_newstate();
	luaL_openlibs(mLuaState);

	lua_pushlightuserdata(mLuaState, this);
	lua_setglobal(mLuaState, CONTEXT_KEY);

	lua_getglobal(mLuaState, "package");
	lua_getfield(mLuaState, -1, "path");
	lua_pushliteral(mLuaState, LUA_MODULE_ENGINE_PATH);
	lua_concat(mLuaState, 2);
	lua_setfield(mLuaState, -2, "path");

	LuaFunctionBinding::BuildContext(*this);
}


LuaContext::~LuaContext(void)
{
	lua_close(mLuaState);
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
		
		// inialize class
		lua_pushinteger(mLuaState, MethodTableIndex);
		lua_newtable(mLuaState);
		lua_rawset(mLuaState, -3);
	
		lua_pushinteger(mLuaState, GetterTableIndex);
		lua_newtable(mLuaState);
		lua_rawset(mLuaState, -3);
	
		lua_pushinteger(mLuaState, SetterTableIndex);
		lua_newtable(mLuaState);
		lua_rawset(mLuaState, -3);
	
		lua_pushliteral(mLuaState, "__index");
		lua_pushcfunction(mLuaState, LuaObjectIndexMethod);
		lua_rawset(mLuaState, -3);

		lua_pushliteral(mLuaState, "__newindex");
		lua_pushcfunction(mLuaState, LuaObjectNewIndexMethod);
		lua_rawset(mLuaState, -3);
	}
	
	// stack = baseclass | class

	// set the class name
	lua_pushliteral(mLuaState, "name");
	lua_pushstring(mLuaState, name.c_str());
	lua_rawset(mLuaState, -3);

	lua_pushliteral(mLuaState, "base");
	// copy base class
	lua_pushvalue(mLuaState, -3);
	// set base class
	lua_rawset(mLuaState, -3);

	// remove base class
	lua_remove(mLuaState, -2);
	
	// stack = class

	lua_pushvalue(mLuaState, -1);
	lua_setfield(mLuaState, -3, name.c_str());
}

void LuaContext::AddMethod(const std::string& name, lua_CFunction method)
{
	AddFunctionToTable(name, method, MethodTableIndex);
}

void LuaContext::AddProperty(const std::string& name, lua_CFunction getter, lua_CFunction setter)
{
	AddFunctionToTable(name, getter, GetterTableIndex);
	AddFunctionToTable(name, setter, SetterTableIndex);
}

void LuaContext::AddMetaMethod(const std::string& name, lua_CFunction method)
{
	lua_pushstring(mLuaState, name.c_str());
	lua_pushcfunction(mLuaState, method);
	lua_rawset(mLuaState, -3);
}

void LuaContext::AddFunctionToTable(const std::string& name, lua_CFunction method, lua_Integer table)
{
	if (table != NULL)
	{
		assert(lua_istable(mLuaState, -1));
		// put the table on the top of the stack
		lua_pushinteger(mLuaState, table);
		lua_rawget(mLuaState, -2);
		// set the method in the table
		lua_pushstring(mLuaState, name.c_str());
		lua_pushcfunction(mLuaState, method);
		lua_rawset(mLuaState, -3);

		// restore the stack
		lua_pop(mLuaState, 1);
	}
}

void LuaContext::GetFunctionFromTable(lua_State* state, int keyIndex, lua_Integer table)
{
	int positiveKeyIndex = PositiveIndex(state, keyIndex);
	// get the table from the class
	lua_pushinteger(state, table);
	lua_rawget(state, -2);
	// get the function from the table
	lua_pushvalue(state, positiveKeyIndex);
	lua_rawget(state, -2);
	// remove the table
	lua_remove(state, -2);
}

void LuaContext::EndClass()
{
	lua_pop(mLuaState, 1);
}

int LuaContext::PositiveIndex(lua_State* state, int index)
{
	if (index < 0)
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

void LuaContext::RunScript(LuaScript& script)
{
	int result = 0;
	int startStackSize = lua_gettop(mLuaState);

	if (script.GetSource().length() > 0)
	{
		result = luaL_dofile(mLuaState, script.GetSource().c_str());
	}
	else
	{
		result = luaL_dostring(mLuaState, script.GetSourceCode().c_str());
	}

	if (result != 0)
	{
		std::cerr << "Error in lua script: " << lua_tostring(mLuaState, -1) << std::endl;
	}

	lua_settop(mLuaState, startStackSize);
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

void LuaContext::RemovePointerIndex(void* pointer)
{
	mPointerIDMapping.erase(pointer);
}

int LuaContext::LuaObjectIndexMethod(lua_State* state)
{
	// stack = table | key
	if (lua_getmetatable(state, -2))
	{
		// stack = table | key | metatable of table
		GetFunctionFromTable(state, -2, MethodTableIndex);

		// stack = table | key | metatable | function
		if (!lua_isnil(state, -1))
		{
			// move function over the table
			lua_copy(state, -1, -4);
			// remove key, metatable, and function
			lua_pop(state, 3);
			// return function
			return 1;
		}

		lua_pop(state, 1);
		// stack = table | key | metatable

		GetFunctionFromTable(state, -2, GetterTableIndex);
		
		// stack = table | key | metatable | function
		if (!lua_isnil(state, -1))
		{
			// move function to the top of the stack
			lua_insert(state, -4);
			// stack = function | table | key | metatable

			// remove key and metatable
			lua_pop(state, 2);

			int beforeCallSize = lua_gettop(state) - 2;
			lua_call(state, 1, LUA_MULTRET);
			int numberReturned = lua_gettop(state) - beforeCallSize;
			
			return numberReturned;
		}

		// pop the metatable and the function
		lua_pop(state, 2);

		string errorMessage("Object does not have given key");

		if (lua_type(state, -1) == LUA_TSTRING)
		{
			errorMessage += ": ";
			errorMessage += lua_tostring(state, -1);
		}

		lua_pop(state, 2);
		lua_pushstring(state, errorMessage.c_str());
		lua_error(state);
		return 0;
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
		GetFunctionFromTable(state, -3, SetterTableIndex);
		
		// stack = table | key | value | metatable | setter
		if (!lua_isnil(state, -1))
		{
			// move the setter into the top of the stack
			lua_insert(state, -5);
			// move value to be set into the second parameter
			lua_copy(state, -2, -3);
			// pop the second value, metatable
			lua_pop(state, 2);

			lua_call(state, 2, 0);
			return 0;
		}

		// remove metatable and setter
		lua_pop(state, 2);

		string errorMessage("Object has no setter for given key ");
		
		if (lua_type(state, -2) == LUA_TSTRING)
		{
			errorMessage += ": ";
			errorMessage += lua_tostring(state, -2);
		}

		lua_pop(state, 3);
		lua_pushstring(state, errorMessage.c_str());
		lua_error(state);
		return 0;
	}
	else
	{
		lua_pop(state, 3);
		lua_pushliteral(state, "No metatable set on object");
		lua_error(state);
		return 0;
	}
}

const lua_Integer LuaContext::MethodTableIndex = 1;
const lua_Integer LuaContext::GetterTableIndex = 2;
const lua_Integer LuaContext::SetterTableIndex = 3;

const char* LuaContext::NoBaseClass = "";

}