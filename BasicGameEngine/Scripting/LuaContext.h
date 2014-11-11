#pragma once

#include "Lua/lua.hpp"
#include "LuaScript.h"
#include <string>
#include <map>
#include <Krono.h>
#include <cassert>
#include <unordered_map>

namespace kge
{
	
struct LuaWeakRefUserData
{
	LuaWeakRefUserData(const krono::Object::Ref& ref, void* ptr);
	~LuaWeakRefUserData();

	krono::Object::Ref ref;
	void* ptr;
};

class LuaContext
{
public:
	LuaContext(void);
	~LuaContext(void);

	lua_State* GetState();

	template<typename T>
	void BeginUserDataClass(const std::string& name, const std::string& baseClass)
	{
		BeginUserDataClass(name, baseClass, typeid(T));
	}

	void BeginUserDataClass(const std::string& name, const std::string& baseClass, const std::type_info& classType);

	void BeginClass(const std::string& name, const std::string& baseClass);
	void AddMethod(const std::string& name, lua_CFunction method);
	void AddProperty(const std::string& name, lua_CFunction getter, lua_CFunction setter);
	void AddMetaMethod(const std::string& name, lua_CFunction method);
	void EndClass();

	static int PositiveIndex(lua_State* state, int index);
	void DeepCopy(int index);

	static void PushVector3(lua_State* state, const krono::Vector3f& vector);
	static void PushQuaternion(lua_State* state, const krono::Quaternionf& quaternion);
	
	static krono::Vector3f ToVector3(lua_State* state, int index);
	static krono::Quaternionf ToQuaternion(lua_State* state, int index);

	template <typename T>
	size_t PushPointer(const krono::Object::Ptr& value)
	{
		// set the meta table
		auto luaClassName = mTypeMapping.find(typeid(T).hash_code());

		if (luaClassName != mTypeMapping.end())
		{
			return PushPointer(value, luaClassName->second);
		}
		else
		{
			return PushPointer(value, "");
		}
	}

	size_t PushPointer(const krono::Object::Ptr& value, const std::string& luaClassName);
	void PushExistingPointer(size_t id);

	template <typename T>
	static std::shared_ptr<T> GetPointer(lua_State* state, int index)
	{
		return std::dynamic_pointer_cast<T>(GetPointer(state, index));
	}

	static krono::Object::Ptr GetPointer(lua_State* state, int index);

	static void PushPointer(lua_State* state, const krono::Object::Ptr& value);
	static krono::Object::Ptr ToPointer(lua_State* state, int index);


	template <typename T>
	size_t PushReference(const krono::Object::Ref& value)
	{
		// set the meta table
		auto luaClassName = mTypeMapping.find(typeid(T).hash_code());

		if (luaClassName != mTypeMapping.end())
		{
			return PushReference(value, luaClassName->second);
		}
		else
		{
			return PushReference(value, "");
		}
	}

	size_t PushReference(const krono::Object::Ref& value, const std::string& luaClassName);
	void PushExistingReference(size_t id);

	template <typename T>
	static std::shared_ptr<T> GetReferencePtr(lua_State* state, int tableIndex)
	{
		return std::dynamic_pointer_cast<T>(GetObjectReference(state, tableIndex).lock());
	}
	
	// gets the object reference from a table
	static krono::Object::Ref GetObjectReference(lua_State* state, int tableIndex);
	
	static void PushWeakRef(lua_State* state, const krono::Object::Ref& value);
	static krono::Object::Ref ToWeakRef(lua_State* state, int index);

	static LuaContext* ContextFromState(lua_State* state);
	static const char* NoBaseClass;

	void RunScript(LuaScript& script);
	
	static int WeakReferenceGC(lua_State* state);
	static int SharedPtrGC(lua_State* state);

	static void GetKGEField(lua_State* state, const char* fieldName);
private:
	void RemovePointerIndex(void* pointer);

	// prevent copying of context
	LuaContext(const LuaContext&);
	void operator=(const LuaContext&);

	lua_State* mLuaState;

	std::unordered_map<size_t, std::string> mTypeMapping;
	std::unordered_map<void*, size_t> mPointerIDMapping;
	size_t mCurrentObjectID;

	void AddFunctionToTable(const std::string& name, lua_CFunction method, lua_Integer table);
	static void GetFunctionFromTable(lua_State* state, int keyIndex, lua_Integer table);

	static int LuaObjectIndexMethod(lua_State* state);
	static int LuaObjectNewIndexMethod(lua_State* state);
	
	static const lua_Integer MethodTableIndex;
	static const lua_Integer GetterTableIndex;
	static const lua_Integer SetterTableIndex;
};

#define OBJECT_REFERENCE_KEY	"_kge_obj"
// used to cache weak_ptr objects in lua
#define ALL_REFERENCES_KEY		"_ref"
// used to cache shared_ptr objects in lua
#define ALL_POINTERS_KEY		"_ptr"
// used to allow shared_ptr objects to be garbage collected
#define CONTEXT_KEY				"_context"

#define LUA_MODULE_ENGINE_PATH	";Engine/Lua/?/init.lua;Engine/Lua/?.lua"

#define KGE_CPP_MODUlE_NAME		"kgecpp"

#define KGE_BEHAVIORS_NAME		"behaviors"

}
