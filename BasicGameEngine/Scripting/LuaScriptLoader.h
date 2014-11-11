#pragma once

#include <Krono.h>

namespace kge
{

class LuaScriptLoader : public krono::ResourceLoader
{
public:
	LuaScriptLoader(void);
	~LuaScriptLoader(void);
	
	virtual Auto<krono::Resource> LoadResource(krono::ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);
private:
};

}