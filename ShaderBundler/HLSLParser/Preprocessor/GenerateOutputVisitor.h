#pragma once

#include "NodeVisitor.h"
#include "Macro.h"
#include "IncludeHandler.h"
#include <sstream>
#include <string>
#include <map>
#include <vector>

namespace preproc
{

class GenerateOutputVisitor : public NodeVisitor
{
public:
	static std::string Process(Node& rootNode, MacroStorage& macroDB, IncludeHandler& includeHandler);

	~GenerateOutputVisitor(void);

	virtual void Visit(Node& node);

	virtual void Visit(OtherNode& node);

	virtual void Visit(DefineNode& node);
	virtual void Visit(IfNode& node);
	virtual void Visit(IncludeNode& node);
	virtual void Visit(LineNode& node);
	virtual void Visit(PragmaNode& node);
	virtual void Visit(UnDefNode& node);
	
	virtual void Visit(IdentifierNode& node);
	virtual void Visit(FunctionNode& node);
	virtual void Visit(ConstantNode& node);

private:
	struct MacroParameterStorage
	{
		std::map<std::string, std::string> macroValues;
	};

	GenerateOutputVisitor(MacroStorage& macroDB, IncludeHandler& includeHandler);

	MacroStorage& mMacroDB;
	IncludeHandler& mIncludeHandler;
	std::ostringstream mOutput;

	std::vector<MacroParameterStorage> mMacroVariableStack;
};

}