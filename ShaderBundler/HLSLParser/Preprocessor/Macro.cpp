#include "Macro.h"

namespace preproc
{

Macro::Macro(const std::shared_ptr<Node>& value) :
	mHasParameters(false),
	mValue(value)
{

}

Macro::Macro(const std::vector<std::string>& parameters, const std::shared_ptr<Node>& value) :
	mHasParameters(true),
	mParameterNames(parameters),
	mValue(value)
{

}

Macro::~Macro(void)
{
}

bool Macro::HasParameters() const
{
	return mHasParameters;
}

const std::vector<std::string>& Macro::GetParameters() const
{
	return mParameterNames;
}

const std::shared_ptr<Node>& Macro::GetValue() const
{
	return mValue;
}

void MacroStorage::DefineMacro(const std::string& name, const std::shared_ptr<Node>& value)
{
	mMacros[name] = std::shared_ptr<Macro>(new Macro(value));
}

void MacroStorage::DefineMacro(const std::string& name, const std::vector<std::string>& parameters, const std::shared_ptr<Node>& value)
{
	mMacros[name] = std::shared_ptr<Macro>(new Macro(parameters, value));
}

void MacroStorage::UndefineMacro(const std::string& name)
{
	auto location = mMacros.find(name);

	if (location != mMacros.end())
	{
		mMacros.erase(location);
	}
}

std::shared_ptr<Macro> MacroStorage::GetMacro(const std::string& name) const
{
	auto it = mMacros.find(name);

	if (it != mMacros.end())
	{
		return it->second;
	}
	else
	{
		return std::shared_ptr<Macro>(NULL);
	}
}

}
