#pragma once

#include "LuaCppConvert.h"

namespace kge
{

namespace detail
{
	template <std::size_t... Is>
	struct Indices {};

	// Declare primary template for index range builder
	template <size_t MIN, size_t N, size_t... Is>
	struct IndicesBuilder;

	// Base step
	template <size_t MIN, size_t... Is>
	struct IndicesBuilder<MIN, MIN, Is...>
	{
		typedef Indices<Is...> Type;
	};

	// Induction step
	template <size_t MIN, size_t N, size_t... Is>
	struct IndicesBuilder : public IndicesBuilder<MIN, N - 1, N - 1, Is...>
	{

	};
	
	template <typename ReturnType, typename... Args, size_t... ArgIndex>
	void ExpandCall(lua_State* state, ReturnType (*function)(Args...), Indices<ArgIndex...>)
	{
		LuaCppConvert::ConvertToLua(state, function(
			(LuaCppConvert::ConvertToCpp<LuaCppConvert::ConstRemove<Args>::ConstFreeType>(state, ArgIndex + 1))...
			));
	}
	
	template <typename ReturnType, typename ClassName, typename... Args, size_t... ArgIndex>
	void ExpandMethodCall(lua_State* state, ClassName* instance, ReturnType (ClassName::*function)(Args...), Indices<ArgIndex...>)
	{
		LuaCppConvert::ConvertToLua(state, (instance->*function)(
			(LuaCppConvert::ConvertToCpp<LuaCppConvert::ConstRemove<Args>::ConstFreeType>(state, ArgIndex + 2))...
			));
	}
	
	template <typename ReturnType, typename ClassName, typename... Args, size_t... ArgIndex>
	void ExpandConstMethodCall(lua_State* state, const ClassName* instance, ReturnType (ClassName::*function)(Args...) const, Indices<ArgIndex...>)
	{
		LuaCppConvert::ConvertToLua(state, (instance->*function)(
			(LuaCppConvert::ConvertToCpp<LuaCppConvert::ConstRemove<Args>::ConstFreeType>(state, ArgIndex + 2))...
			));
	}
	
	template <typename... Args, size_t... ArgIndex>
	void ExpandVoidCall(lua_State* state, void (*function)(Args...), Indices<ArgIndex...>)
	{
		function((LuaCppConvert::ConvertToCpp<LuaCppConvert::ConstRemove<Args>::ConstFreeType>(state, ArgIndex + 1))...);
	}
	
	template <typename ClassName, typename... Args, size_t... ArgIndex>
	void ExpandVoidMethodCall(lua_State* state, ClassName* instance, void (ClassName::*function)(Args...), Indices<ArgIndex...>)
	{
		(instance->*function)((LuaCppConvert::ConvertToCpp<LuaCppConvert::ConstRemove<Args>::ConstFreeType>(state, ArgIndex + 2))...);
	}
	
	template <typename ClassName, typename... Args, size_t... ArgIndex>
	void ExpandConstVoidMethodCall(lua_State* state, const ClassName* instance, void (ClassName::*function)(Args...) const, Indices<ArgIndex...>)
	{
		(instance->*function)((LuaCppConvert::ConvertToCpp<LuaCppConvert::ConstRemove<Args>::ConstFreeType>(state, ArgIndex + 2))...);
	}
}

class LuaFunctionBase
{
public:
	virtual ~LuaFunctionBase() {};
	virtual int Apply(lua_State* state) = 0;
};

template <typename ReturnType, typename... Args>
class LuaCFunction : public LuaFunctionBase
{
public:
	typedef ReturnType(*FunctionType)(Args...);

	LuaCFunction(FunctionType functionPointer)
	{
		mFunctionPointer = functionPointer;
	}

	virtual ~LuaCFunction() {}

	virtual int Apply(lua_State* state)
	{
		return MakeCall<ReturnType>(state, NULL);
	}
private:
	template <typename Ret>
	int MakeCall(lua_State* state, Ret* valType)
	{
		detail::ExpandCall(state, mFunctionPointer, detail::IndicesBuilder<0, sizeof...(Args)>::Type());
		return 1;
	}

	template <>
	int MakeCall(lua_State* state, void* valType)
	{
		detail::ExpandVoidCall(state, mFunctionPointer, detail::IndicesBuilder<0, sizeof...(Args)>::Type());
		return 0;
	}

	FunctionType mFunctionPointer;
};

template <typename ReturnType, typename ClassType, typename... Args>
class LuaMethodFunction : public LuaFunctionBase
{
public:
	typedef ReturnType(ClassType::*FunctionType)(Args...);

	LuaMethodFunction(FunctionType functionPointer)
	{
		mFunctionPointer = functionPointer;
	}

	virtual ~LuaMethodFunction() {}

	virtual int Apply(lua_State* state)
	{
		return MakeCall<ReturnType>(state, NULL);
	}
private:

	template <typename Ret>
	int MakeCall(lua_State* state, Ret* valType)
	{
		ClassType *instance = dynamic_cast<ClassType*>(LuaCppConvert::GetObjectPointer(state, 1));
		detail::ExpandMethodCall(state, instance, mFunctionPointer, detail::IndicesBuilder<0, sizeof...(Args)>::Type());
		return 1;
	}

	template <>
	int MakeCall(lua_State* state, void* valType)
	{
		ClassType *instance = dynamic_cast<ClassType*>(LuaCppConvert::GetObjectPointer(state, 1));
		detail::ExpandVoidMethodCall(state, instance, mFunctionPointer, detail::IndicesBuilder<0, sizeof...(Args)>::Type());
		return 0;
	}

	FunctionType mFunctionPointer;
};

template <typename ReturnType, typename ClassType, typename... Args>
class LuaConstMethodFunction : public LuaFunctionBase
{
public:
	typedef ReturnType(ClassType::*FunctionType)(Args...) const;

	LuaConstMethodFunction(FunctionType functionPointer)
	{
		mFunctionPointer = functionPointer;
	}

	virtual ~LuaConstMethodFunction() {}

	virtual int Apply(lua_State* state)
	{
		return MakeCall<LuaCppConvert::ConstRemove<ReturnType>::ConstFreeType>(state, NULL);
	}
private:

	template <typename Ret>
	int MakeCall(lua_State* state, Ret* valType)
	{
		const ClassType *instance = dynamic_cast<ClassType*>(LuaCppConvert::GetObjectPointer(state, 1));
		detail::ExpandConstMethodCall(state, instance, mFunctionPointer, detail::IndicesBuilder<0, sizeof...(Args)>::Type());
		return 1;
	}

	template <>
	int MakeCall(lua_State* state, void* valType)
	{
		const ClassType *instance = dynamic_cast<ClassType*>(LuaCppConvert::GetObjectPointer(state, 1));
		detail::ExpandConstVoidMethodCall(state, instance, mFunctionPointer, detail::IndicesBuilder<0, sizeof...(Args)>::Type());
		return 0;
	}

	FunctionType mFunctionPointer;
};

}