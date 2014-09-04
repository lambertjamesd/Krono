#include "GenerateOutputVisitor.h"
#include "Preprocessor.h"
#include "EvaluateVisitor.h"

using namespace std;

namespace preproc
{

GenerateOutputVisitor::GenerateOutputVisitor(MacroStorage& macroDB, IncludeHandler& includeHandler) :
	mMacroDB(macroDB),
	mIncludeHandler(includeHandler)
{

}

GenerateOutputVisitor::~GenerateOutputVisitor(void)
{

}

std::string GenerateOutputVisitor::Process(Node& rootNode, MacroStorage& macroDB, IncludeHandler& includeHandler)
{
	GenerateOutputVisitor visitor(macroDB, includeHandler);
	rootNode.Accept(visitor);
	return visitor.mOutput.str();
}

void GenerateOutputVisitor::Visit(Node& node)
{

}

void GenerateOutputVisitor::Visit(OtherNode& node)
{
	mOutput << node.GetValue();
}

void GenerateOutputVisitor::Visit(DefineNode& node)
{
	if (node.HasParameters())
	{
		mMacroDB.DefineMacro(node.GetName(), node.GetParameters(), node.GetValue());
	}
	else
	{
		mMacroDB.DefineMacro(node.GetName(), node.GetValue());
	}
}

void GenerateOutputVisitor::Visit(IfNode& node)
{
	GenerateOutputVisitor expressionGenerator(mMacroDB, mIncludeHandler);
	node.GetExpression().Accept(expressionGenerator);

	unique_ptr<ExpressionNode> expression = Preprocessor::ParseExpression(expressionGenerator.mOutput.str());

	EvaluateVisitor evalutator(mMacroDB);
	expression->Accept(evalutator);

	if (evalutator.GetResult())
	{
		node.GetBody().Accept(*this);
	}
	else if (node.GetElse() != NULL)
	{
		node.GetElse()->Accept(*this);
	}
}

void GenerateOutputVisitor::Visit(IncludeNode& node)
{
	char* fileData;
	size_t fileSize;

	mIncludeHandler.Open(node.GetIncludeType(), node.GetFilename().c_str(), (void**)&fileData, &fileSize);

	std::string dataString(fileData, fileSize);
	std::istringstream dataStream(dataString);

	mOutput << Preprocessor::PreprocessStream(dataStream, mMacroDB, mIncludeHandler);

	mIncludeHandler.Close(fileData);

}

void GenerateOutputVisitor::Visit(LineNode& node)
{

}

void GenerateOutputVisitor::Visit(PragmaNode& node)	
{

}

void GenerateOutputVisitor::Visit(UnDefNode& node)
{
	mMacroDB.UndefineMacro(node.GetName());
}

void GenerateOutputVisitor::Visit(DefinedOperatorNode& node)
{
	if (node.DoesUseParenthesis())
	{
		mOutput << "defined(" << node.GetName() << ")";
	}
	else
	{
		mOutput << "defined " << node.GetName();
	}
}

void GenerateOutputVisitor::Visit(IdentifierNode& node)
{
	if (mMacroVariableStack.size())
	{
		auto macroParameter = mMacroVariableStack.back().macroValues.find(node.GetValue());

		if (macroParameter != mMacroVariableStack.back().macroValues.end())
		{
			mOutput << macroParameter->second;
		}

		return;
	}

	std::shared_ptr<Macro> macro = mMacroDB.GetMacro(node.GetValue());

	if (macro != NULL)
	{
		if (macro->HasParameters())
		{
			std::string message;
			message += "The macro needs to be a function ";
			message += node.GetValue();
			throw Exception(message.c_str());
		}
		else
		{
			macro->GetValue()->Accept(*this);
		}
	}
	else
	{
		mOutput << node.GetValue();
	}
}

void GenerateOutputVisitor::Visit(FunctionNode& node)
{
	std::shared_ptr<Macro> macro = mMacroDB.GetMacro(node.GetName());

	if (macro != NULL)
	{
		if (macro->HasParameters())
		{
			if (macro->GetParameters().size() != node.GetParameterCount())
			{
				throw Exception("Incorrect number of parameters in macro");
			}

			MacroParameterStorage parameterStorage; 

			for (size_t i = 0; i < node.GetParameterCount(); ++i)
			{
				GenerateOutputVisitor subGenerater(mMacroDB, mIncludeHandler);
				node.GetParameter(i)->Accept(subGenerater);
				parameterStorage.macroValues[macro->GetParameters()[i]] = subGenerater.mOutput.str();
			}

			mMacroVariableStack.push_back(parameterStorage);
			macro->GetValue()->Accept(*this);
			mMacroVariableStack.pop_back();

			return;
		}
		else
		{
			macro->GetValue()->Accept(*this);
		}
	}
	else
	{
		mOutput << node.GetName();
	}

	mOutput << "(";

	for (size_t i = 0; i < node.GetParameterCount(); ++i)
	{
		if (i > 0)
		{
			mOutput << ",";
		}

		node.GetParameter(i)->Accept(*this);
	}

	mOutput << ")";
}

}
