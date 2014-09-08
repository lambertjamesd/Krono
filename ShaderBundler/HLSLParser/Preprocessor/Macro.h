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
	Macro(const std::shared_ptr<Node>& value);
	Macro(const std::vector<std::string>& parameters, const std::shared_ptr<Node>& value);
	virtual ~Macro(void);

	bool HasParameters() const;
	const std::vector<std::string>& GetParameters() const;
	const std::shared_ptr<Node>& GetValue() const;
private:
	bool mHasParameters;
	std::vector<std::string> mParameterNames;
	std::shared_ptr<Node> mValue;
};

class MacroStorage
{
public:
	void DefineMacro(const std::string& name, const std::string& value);
	void DefineMacro(const std::string& name, const std::shared_ptr<Node>& value);
	void DefineMacro(const std::string& name, const std::vector<std::string>& parameters, const std::shared_ptr<Node>& value);
	void UndefineMacro(const std::string& name);

	std::shared_ptr<Macro> GetMacro(const std::string& name) const;
private:
	std::map<std::string, std::shared_ptr<Macro> > mMacros;
};

}
