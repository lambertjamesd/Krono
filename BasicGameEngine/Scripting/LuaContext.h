#pragma once

#include "Lua.h"
#include "LuaFunctionBase.h"
#include "LuaScript.h"
#include <string>
#include <map>
#include <Krono.h>
#include <cassert>
#include <unordered_map>
#include <tuple>

#include "GameObject/Transform.h"

#include "LuaFunctionBase.h"

namespace kge
{
	
struct LuaWeakRefUserData
{
	LuaWeakRefUserData(const krono::Object::Ref& ref, void* ptr);
	~LuaWeakRefUserData();

	krono::Object::Ref ref;
	void* ptr;
};

class LuaErrorException : public krono::Exception
{
public:
	LuaErrorException(const std::string& message);
private:
};

class LuaMethodNotFoundException : public krono::Exception
{
public:
	LuaMethodNotFoundException(const std::string& message);
};

class LuaFunctionCall
{
public:
	template <typename ReturnType, typename... Args>
	static ReturnType CallMethod(lua_State* state, const char* methodName, Args&&... parameters)
	{
		return Call<ReturnType, Args&&...>(state, methodName, false, parameters...);
	}
	
	template <typename ReturnType, typename... Args>
	static ReturnType CallStaticMethod(lua_State* state, const char* methodName, Args&&... parameters)
	{
		return Call<ReturnType, Args&&...>(state, methodName, true, parameters...);
	}
private:
	template <typename ReturnType>
	static ReturnType Return(lua_State* state)
	{
		ReturnType result = LuaCppConvert::ConvertToCpp<ReturnType>(state, -1);
		lua_pop(state, 1);
		return result;
	}

	template <>
	static void Return(lua_State* state)
	{

	}

	template <>
	static LuaCppConvert::ReturnToStack Return(lua_State* state)
	{
		return LuaCppConvert::ReturnToStack();
	}

	static void Pass(lua_State* state)
	{

	}

	template<typename First, typename... Remaining>
	static void Pass(lua_State* state, const First& first, Remaining&&... parameters)
	{
		LuaCppConvert::ConvertToLua(state, first);
		Pass(state, parameters...);
	}

	template <typename ReturnType, typename... Args>
	static ReturnType Call(lua_State* state, const char* methodName, bool isStatic, Args&&... parameters)
	{
		int startingTop = lua_gettop(state);

		if (methodName != NULL)
		{
			lua_getfield(state, -1, methodName);
		}
		else
		{
			assert(isStatic && "Unamed function must be a static method");
		}

		int parameterCount = sizeof...(Args);
		
		if (!isStatic)
		{
			lua_pushvalue(state, -2);
			++parameterCount;
		}

		Pass(state, parameters...);

		int callResult = lua_pcall(state, parameterCount, LuaCppConvert::ReturnTypeCount<ReturnType>::Value, 0);
		
		if (callResult != 0)
		{
			std::string message("Error running method ");

			if (methodName != NULL)
			{
				message += methodName;
			}

			message += ": ";

			if (lua_isstring(state, -1))
			{
				message += lua_tostring(state, -1);
			}

			lua_settop(state, startingTop);
			
			throw LuaErrorException(message);
		}

		return Return<ReturnType>(state);
	}
};

class LuaContext
{
public:
	LuaContext(void);
	~LuaContext(void);

	bool Require(const char* moduleName);

	lua_State* GetState();
	
	template <typename ReturnType, typename... Args>
	ReturnType CallMethod(const char* methodName, Args&&... parameters)
	{
		return LuaFunctionCall::CallMethod<ReturnType>(mLuaState, methodName, parameters...);
	}
	
	template <typename ReturnType, typename... Args>
	ReturnType CallStaticMethod(const char* methodName, Args&&... parameters)
	{
		return LuaFunctionCall::CallStaticMethod<ReturnType>(mLuaState, methodName, parameters...);
	}

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
	
	void SetMethod(const std::string& name);
	void SetGetter(const std::string& name);
	void SetSetter(const std::string& name);

	void EndClass();

	static int PositiveIndex(lua_State* state, int index);
	void DeepCopy(int index);
	void ShallowCopy(int index);

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

	static bool IsWeakPointer(lua_State* state, int index);

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
	
	size_t PushReference(const krono::Object::Ref& value);
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

	void AddBehaviorScript(const LuaScript::Ptr& script);
	void AddBehaviorScript(const LuaScript::Ptr& script, const std::string& behaviorName);

	bool LoadClassTable(const std::string& value);
	
	static int WeakReferenceGC(lua_State* state);
	static int SharedPtrGC(lua_State* state);

	static void GetKGEField(lua_State* state, const char* fieldName);
	
	template <typename ReturnType, typename... Args>
	void PushCFunction(ReturnType(*function)(Args...))
	{
		std::unique_ptr<LuaFunctionBase> functionBase(new LuaCFunction<ReturnType, Args...>(function));

		lua_pushlightuserdata(mLuaState, (void*)static_cast<LuaFunctionBase*>(functionBase.get()));
		lua_pushcclosure(mLuaState, &LuaCallFunctionBase, 1);

		mBoundFunctions.push_back(move(functionBase));
	}

	template <typename ReturnType, typename ClassName, typename... Args>
	void PushClassMethod(ReturnType(ClassName::*function)(Args...))
	{
		std::unique_ptr<LuaFunctionBase> functionBase(new LuaMethodFunction<ReturnType, ClassName, Args...>(function));

		lua_pushlightuserdata(mLuaState, (void*)static_cast<LuaFunctionBase*>(functionBase.get()));
		lua_pushcclosure(mLuaState, &LuaCallFunctionBase, 1);

		mBoundFunctions.push_back(move(functionBase));
	}

	template <typename ReturnType, typename ClassName, typename... Args>
	void PushClassMethod(ReturnType(ClassName::*function)(Args...) const)
	{
		std::unique_ptr<LuaFunctionBase> functionBase(new LuaConstMethodFunction<ReturnType, ClassName, Args...>(function));

		lua_pushlightuserdata(mLuaState, (void*)static_cast<LuaFunctionBase*>(functionBase.get()));
		lua_pushcclosure(mLuaState, &LuaCallFunctionBase, 1);

		mBoundFunctions.push_back(move(functionBase));
	}

	void ConnectDebugger(const char* host = "localhost", unsigned short port = 8172);
	void PauseDebugger();
	void ResumeDebugger();
private:
	void RemovePointerIndex(void* pointer);

	static bool IsProperty(lua_State* state, int index);
	static bool PushPropertyFunction(lua_State* state, int index, const char* functionName);
	static bool PushSetter(lua_State* state, int index);

	void SetPropertyField(const std::string& name, const char* fieldName);

	// prevent copying of context
	LuaContext(const LuaContext&);
	void operator=(const LuaContext&);

	lua_State* mLuaState;

	std::unordered_map<size_t, std::string> mTypeMapping;
	std::unordered_map<void*, size_t> mPointerIDMapping;
	size_t mCurrentObjectID;

	std::vector<std::unique_ptr<LuaFunctionBase> > mBoundFunctions;

	static int LuaCallFunctionBase(lua_State* state);

	static int LuaObjectIndexMethod(lua_State* state);
	static int LuaObjectNewIndexMethod(lua_State* state);
	
	bool mDebuggerLoaded;
	bool mDebuggerConnected;
	bool mDebuggerEnabled;
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

#define KGE_IS_PROPERTY_KEY		"__kge_isprop"

#define KGE_GETTER_KEY			"get"
#define KGE_SETTER_KEY			"set"

}
