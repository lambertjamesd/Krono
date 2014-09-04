#pragma once

#include <map>
#include <vector>
#include <string>
#include <memory>
#include "ExpressionNode.h"

namespace preproc
{

class Macro
{
public:
	Macro(const std::shared_ptr<Node>& value, const std::shared_ptr<ExpressionNode>& expressionValue);
	Macro(const std::vector<std::string>& parameters, const std::shared_ptr<Node>& value, const std::shared_ptr<ExpressionNode>& expressionValue);
	virtual ~Macro(void);

	bool HasParameters() const;
	const std::vector<std::string>& GetParameters() const;
	const std::shared_ptr<Node>& GetValue() const;
	const std::shared_ptr<ExpressionNode>& GetExpressionValue() const;
private:
	bool mHasParameters;
	std::vector<std::string> mParameterNames;
	std::shared_ptr<Node> mValue;
	std::shared_ptr<ExpressionNode> mExpressionValue;
};

class MacroStorage
{
public:
	void DefineMacro(const std::string& name, const std::shared_ptr<Node>& value, const std::shared_ptr<ExpressionNode>& expressionValue);
	void DefineMacro(const std::string& name, const std::vector<std::string>& parameters, const std::shared_ptr<Node>& value, const std::shared_ptr<ExpressionNode>& expressionValue);
	void UndefineMacro(const std::string& name);

	std::shared_ptr<Macro> GetMacro(const std::string& name) const;
private:
	std::map<std::string, std::shared_ptr<Macro> > mMacros;
};

}