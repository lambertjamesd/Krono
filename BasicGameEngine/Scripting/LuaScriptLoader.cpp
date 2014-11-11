#include "LuaScriptLoader.h"
#include "LuaScript.h"

namespace kge
{

LuaScriptLoader::LuaScriptLoader(void)
{
}


LuaScriptLoader::~LuaScriptLoader(void)
{
}

Auto<krono::Resource> LuaScriptLoader::LoadResource(krono::ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName)
{
	return Auto<LuaScript>(new LuaScript(ResourceLoader::StringFromIStream(inputStream)));
}

}