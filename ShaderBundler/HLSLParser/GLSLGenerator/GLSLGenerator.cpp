#include "GLSLGenerator.h"
#include "HLSLParser/HLSLParser.h"
#include "HLSLParser/Preprocessor/Preprocessor.h"
#include "HLSLParser/HLSLParserException.h"
#include "GLSLTypeStorage.h"
#include <sstream>

using namespace std;

GLSLPostIndexGenerator::GLSLPostIndexGenerator(std::ostream& output, GLSLTypeStorage& typeStorage) :
	mOutput(output),
	mTypeStorage(typeStorage)
{

}

void GLSLPostIndexGenerator::Visit(HLSLNode& node)
{
	// do nothing
}

void GLSLPostIndexGenerator::Visit(HLSLNamedTypeNode& node)
{
	mTypeStorage.GetValue(node.GetName())->Accept(*this);
}

void GLSLPostIndexGenerator::Visit(HLSLArrayTypeNode& node)
{
	mOutput << '[' << node.GetSize() << ']';
}


////////////////////////////////////////////////

GLSLGenerator::GLSLGenerator(std::ostream& output) :
	mTypeStorage(new GLSLTypeStorage()),
	mOutput(output),
	mIndentLevel(0),
	mNoSemiColon(false)
{

}

GLSLGenerator::GLSLGenerator(std::ostream& output, GLSLTypeStorage* typeStorage) :
	mTypeStorage(typeStorage),
	mOutput(output)
{

}

GLSLGenerator::~GLSLGenerator(void)
{
}

void GLSLGenerator::ProcessFile(const std::string& filename, std::ostream& output)
{
	map<string, string> macros;
	macros["OPENGL"] = "1";

	preproc::PreprocessResult preprocessResult = preproc::Preprocessor::PreprocessFile(filename, macros);
	std::istringstream input(preprocessResult.text);
	HLSLParser parser(input);

	output << "#version 440" << endl;

	GLSLGenerator generator(output);
	parser.ParseFile()->Accept(generator);
}

void GLSLGenerator::Visit(HLSLNodeList& node)
{
	for (size_t i = 0; i < node.GetSize(); ++i)
	{
		OutputIndents();
		node.GetElement(i).Accept(*this);
		if (mNoSemiColon)
		{
			mNoSemiColon = false;
		}
		else
		{
			mOutput << ';';
		}

		mOutput << endl;
	}
}

void GLSLGenerator::Visit(HLSLStatementBlock& node)
{
	mOutput << endl;
	OutputIndents();
	mOutput << "{" << endl;
	IncreaseIndent();
	Visit(static_cast<HLSLNodeList&>(node));
	DecreaseIndent();
	OutputIndents();
	mOutput << "}";

	mNoSemiColon = true;
}

void GLSLGenerator::Visit(HLSLTypedefDefinition& node)
{
	mTypeStorage->Define(node.GetName(), node.GetType());
	mNoSemiColon = true;
}

void GLSLGenerator::Visit(HLSLVariableDefinition& node)
{
	OutputVariableDeclaration(node.GetName(), node.GetType());

	if (node.GetInitialValue() != NULL)
	{
		mOutput << " = ";
		node.GetInitialValue()->Accept(*this);
	}

	mTypeStorage->Define(node.GetName(), node.GetTypePtr());
}

void GLSLGenerator::Visit(HLSLFunctionParameter& node)
{
	switch (node.GetInputModifier())
	{
	case HLSLFunctionParameter::None: break;
	case HLSLFunctionParameter::In: mOutput << "in "; break;
	case HLSLFunctionParameter::Out: mOutput << "out "; break;
	case HLSLFunctionParameter::InOut: mOutput << "inout "; break;
	default:
		throw HLSLParserException(node.GetToken(), "unsuported parameter modifier");
	}

	OutputVariableDeclaration(node.GetName(), node.GetType());
}

void GLSLGenerator::Visit(HLSLFunctionDefinition& node)
{
	OutputVariableDeclaration("", node.GetReturnType());
	mOutput << node.GetName() << "(";

	bool isFirst = true;

	for (size_t i = 0; i < node.GetParameterCount(); ++i)
	{
		if (isFirst)
		{
			isFirst = false;
		}
		else
		{
			mOutput << ", ";
		}

		node.GetParameter(i).Accept(*this);
	}

	mOutput << ")";

	mTypeStorage->DefineFunction(node);
	
	if (node.GetBody() != NULL)
	{
		node.GetBody()->Accept(*this);
	}
}

void GLSLGenerator::Visit(HLSLReturnStatement& node)
{
	mOutput << "return ";
	node.GetReturnValue().Accept(*this);
}

void GLSLGenerator::Visit(HLSLStructureMember& node)
{
	OutputVariableDeclaration(node.GetName(), node.GetType());
}

void GLSLGenerator::Visit(HLSLStructDefinition& node)
{
	mOutput << "struct " << node.GetName() << endl;
	OutputIndents();
	mOutput << "{" << endl;

	IncreaseIndent();
	mTypeStorage->BeginScope();

	for (size_t i = 0; i < node.GetMemberCount(); ++i)
	{
		OutputIndents();
		node.GetMember(i).Accept(*this);
		mOutput << ";" << endl;
	}

	mTypeStorage->EndScope();
	DecreaseIndent();

	OutputIndents();
	mOutput << "}";

	mTypeStorage->Define(node.GetName(), shared_ptr<HLSLNode>(new HLSLStructTypeNode(node)));

	mNoSemiColon = false;
}

void GLSLGenerator::Visit(HLSLCBufferDefinition& node)
{
	if (node.GetRegisterLocation().HasValue())
	{
		mOutput << "layout(binding = " << node.GetRegisterLocation().GetIndex() << ") ";
	}

	mOutput << "uniform " << node.GetName() << endl;

	OutputIndents();

	mOutput << "{" << endl;

	IncreaseIndent();

	for (size_t i = 0; i < node.GetMemberCount(); ++i)
	{
		OutputIndents();
		node.GetMember(i).Accept(*this);
		mOutput << ";" << endl;
	}

	DecreaseIndent();

	OutputIndents();
	mOutput << "}";

	mNoSemiColon = false;
}

void GLSLGenerator::OutputVariableDeclaration(const std::string& variableName, HLSLTypeNode& type)
{
	type.Accept(*this);
	mOutput << " " << variableName;
	GLSLPostIndexGenerator indexGenerator(mOutput, *mTypeStorage);
	type.Accept(indexGenerator);
}

void GLSLGenerator::Visit(HLSLVoidNode& node)
{
	mOutput << "void";
}

void GLSLGenerator::Visit(HLSLNamedTypeNode& node)
{
	HLSLNode* type = mTypeStorage->GetValue(node.GetToken().GetValue());
	
	if (type)
	{
		type->Accept(*this);
	}
	else
	{
		throw HLSLParserException(node.GetToken(), "undefined type");
	}
}

void GLSLGenerator::Visit(HLSLScalarTypeNode& node)
{
	switch (node.GetToken().GetKeywordType())
	{
	case HLSLKeyword::Uint:
	case HLSLKeyword::Dword:
		mOutput << "uint";
		break;
	case HLSLKeyword::Bool:
	case HLSLKeyword::Int:
	case HLSLKeyword::Half:
	case HLSLKeyword::Float:
	case HLSLKeyword::Double:
		mOutput << node.GetToken().GetValue();
		break;
	default:
		throw HLSLParserException(node.GetToken(), "unsuported primitive type");
	}
}

void GLSLGenerator::Visit(HLSLBufferTypeNode& node)
{
	throw HLSLParserException(node.GetToken(), "buffer type unsupported");
}

void GLSLGenerator::Visit(HLSLArrayTypeNode& node)
{
	node.GetInternalType().Accept(*this);
}

std::string GLSLGenerator::GetScalarPrefix(HLSLTypeNode& node)
{
	ostringstream scalarTypeName;
	GLSLGenerator scalarTypeGenerator(scalarTypeName);
	node.Accept(scalarTypeGenerator);

	string scalarStringName = scalarTypeName.str();
	string scalarPrefix;

	if (scalarStringName == "bool")
	{
		scalarPrefix = "b";
	}
	else if (scalarStringName == "int")
	{
		scalarPrefix = "i";
	}
	else if (scalarStringName == "uint")
	{
		scalarPrefix = "u";
	}
	else if (scalarStringName == "half")
	{
		scalarPrefix = "";
	}
	else if (scalarStringName == "float")
	{
		scalarPrefix = "";
	}
	else if (scalarStringName == "double")
	{
		scalarPrefix = "d";
	}
	else
	{
		throw HLSLParserException(node.GetToken(), "invalid vector type");
	}

	return scalarPrefix;
}

void GLSLGenerator::Visit(HLSLVectorTypeNode& node)
{
	mOutput << GetScalarPrefix(node.GetInternalType()) << "vec" << node.GetSize();
}

void GLSLGenerator::Visit(HLSLMatrixTypeNode& node)
{
	if (node.GetColumns() == node.GetRows())
	{
		mOutput << GetScalarPrefix(node.GetInternalType()) << "mat" << node.GetColumns();
	}
	else
	{
		mOutput << GetScalarPrefix(node.GetInternalType()) << "mat" << node.GetRows() << "x" << node.GetColumns();
	}
}

void GLSLGenerator::Visit(HLSLStructTypeNode& node)
{
	mOutput << node.GetName();
}

void GLSLGenerator::Visit(HLSLDoNode& node)
{
	mOutput << "do";

	node.GetBody().Accept(*this);

	OutputIndents();
	mOutput << " while (";
	node.GetCondition().Accept(*this);
	mOutput << ")";

	mNoSemiColon = false;
}

void GLSLGenerator::Visit(HLSLIfNode& node)
{
	mOutput << "if (";
	node.GetCondition().Accept(*this);
	mOutput << ")";

	node.GetBody().Accept(*this);
}

void GLSLGenerator::Visit(HLSLForNode& node)
{
	mOutput << "for (";
	node.GetInitializer().Accept(*this);
	mOutput << "; ";
	node.GetCondition().Accept(*this);
	mOutput << "; ";
	node.GetModifier().Accept(*this);
	mOutput << ")";

	node.GetBody().Accept(*this);
}

void GLSLGenerator::Visit(HLSLSwitchNode& node)
{
	mOutput << "switch (";
	node.GetSelectValue().Accept(*this);
	mOutput << ")";

	node.GetBody().Accept(*this);
}

void GLSLGenerator::Visit(HLSLSwitchLabelNode& node)
{
	mOutput << endl;
	DecreaseIndent();
	OutputIndents();
	mOutput << "case " << node.GetValue() << ":" << endl;
	IncreaseIndent();
	
	mNoSemiColon = true;
}

void GLSLGenerator::Visit(HLSLDefaultNode& node)
{
	mOutput << endl;
	DecreaseIndent();
	OutputIndents();
	mOutput << "default:" << endl;
	IncreaseIndent();

	mNoSemiColon = true;
}

void GLSLGenerator::Visit(HLSLWhileNode& node)
{
	mOutput << "while (";
	node.GetCondition().Accept(*this);
	mOutput << ")";

	node.GetBody().Accept(*this);
}

void GLSLGenerator::Visit(HLSLBreakNode& node)
{
	mOutput << "break";
}

void GLSLGenerator::Visit(HLSLContinueNode& node)
{
	mOutput << "continue";
}

void GLSLGenerator::Visit(HLSLDiscardNode& node)
{
	mOutput << "discard";
}


void GLSLGenerator::Visit(HLSLIdentifierNode& node)
{
	mOutput << node.GetName();
}

void GLSLGenerator::Visit(HLSLNumberConstantNode& node)
{
	mOutput << node.GetStringValue();
}

void GLSLGenerator::Visit(HLSLBooleanConstantNode& node)
{
	mOutput << (node.GetBoolValue() ? "true" : "false");
}

void GLSLGenerator::Visit(HLSLTypeExpressionNode& node)
{
	node.GetType().Accept(*this);
}

void GLSLGenerator::Visit(HLSLParenthesisNode& node)
{
	mOutput << "(";
	node.GetValue().Accept(*this);
	mOutput << ")";
}
	
void GLSLGenerator::Visit(HLSLBinaryOperatorNode& node)
{
	node.GetLeft().Accept(*this);
	mOutput << " " << node.GetToken().GetValue() << " ";
	node.GetRight().Accept(*this);
}
	
void GLSLGenerator::Visit(HLSLUnaryOperator& node)
{
	mOutput << node.GetToken().GetValue();
	node.GetInnerExpression().Accept(*this);
}

void GLSLGenerator::Visit(HLSLCastNode& node)
{
	mOutput << "(";
	node.GetType().Accept(*this);
	mOutput << ")";
	node.GetRight().Accept(*this);
}

void GLSLGenerator::Visit(HLSLPrefixNode& node)
{
	mOutput << node.GetToken().GetValue();
	node.GetRight().Accept(*this);
}

void GLSLGenerator::Visit(HLSLPostfixNode& node)
{
	node.GetLeft().Accept(*this);
	mOutput << node.GetToken().GetValue();
}

void GLSLGenerator::Visit(HLSLIndexNode& node)
{
	node.GetLeft().Accept(*this);
	mOutput << '[';
	node.GetIndex().Accept(*this);
	mOutput << ']';
}

void GLSLGenerator::Visit(HLSLStructureNode& node)
{
	node.GetLeft().Accept(*this);
	mOutput << '.' << node.GetRight();
}

void GLSLGenerator::Visit(HLSLFunctionCallNode& node)
{
	std::ostringstream stringOutput;
	GLSLGenerator nameGenerator(stringOutput, mTypeStorage);
	node.GetLeft().Accept(nameGenerator);

	std::string name = stringOutput.str();

	mOutput << name;

	bool isFirst = true;

	mOutput << '(';

	for (size_t i = 0; i < node.GetParameterCount(); ++i)
	{
		if (isFirst)
		{
			isFirst = false;
		}
		else
		{
			mOutput << ", ";
		}

		node.GetParameter(i).Accept(*this);
	}

	mOutput << ')';
}


void GLSLGenerator::OutputIndents()
{
	for (size_t i = 0; i < mIndentLevel; ++i)
	{
		mOutput << gIndentString;
	}
}

void GLSLGenerator::IncreaseIndent()
{
	++mIndentLevel;
}

void GLSLGenerator::DecreaseIndent()
{
	--mIndentLevel;
}

string GLSLGenerator::gIndentString = "\t";
