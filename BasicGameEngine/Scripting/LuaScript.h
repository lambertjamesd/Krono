#pragma once

#include <string>
#include <Krono.h>

namespace kge
{

class LuaScript : public krono::Resource
{
public:
	typedef std::shared_ptr<LuaScript> Ptr;

	LuaScript(const std::string& sourceCode);
	~LuaScript(void);

	const std::string& GetSourceCode() const;
private:
	std::string mSourceCode;
};

}