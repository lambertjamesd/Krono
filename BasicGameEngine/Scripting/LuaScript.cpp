#include "LuaScript.h"

namespace kge
{

LuaScript::LuaScript(const std::string& sourceCode) :
	mSourceCode(sourceCode)
{
}

LuaScript::~LuaScript(void)
{

}

const std::string& LuaScript::GetSourceCode() const
{
	return mSourceCode;
}

}