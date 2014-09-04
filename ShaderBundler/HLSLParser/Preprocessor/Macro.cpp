#include "Macro.h"

namespace preproc
{

Macro::Macro(const std::shared_ptr<Node>& value, const std::shared_ptr<ExpressionNode>& expressionValue) :
	mHasParameters(false),
	mValue(value),
	mExpressionValue(expressionValue)
{

}

Macro::Macro(const std::vector<std::string>& parameters, const std::shared_ptr<Node>& value, const std::shared_ptr<ExpressionNode>& expressionValue) :
	mHasParameters(false),
	mParameterNames(parameters),
	mValue(value),
	mExpressionValue(expressionValue)
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

const std::shared_ptr<ExpressionNode>& Macro::GetExpressionValue() const
{
	return mExpressionValue;
}

void MacroStorage::DefineMacro(const std::string& name, const std::shared_ptr<Node>& value, const std::shared_ptr<ExpressionNode>& expressionValue)
{
	mMacros[name] = std::shared_ptr<Macro>(new Macro(value, expressionValue));
}

void MacroStorage::DefineMacro(const std::string& name, const std::vector<std::string>& parameters, const std::shared_ptr<Node>& value, const std::shared_ptr<ExpressionNode>& expressionValue)
{
	mMacros[name] = std::shared_ptr<Macro>(new Macro(parameters, value, expressionValue));
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