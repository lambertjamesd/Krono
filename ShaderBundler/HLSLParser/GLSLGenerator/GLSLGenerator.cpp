#include "GLSLGenerator.h"
#include "HLSLParser/HLSLParser.h"
#include "HLSLParser/Preprocessor/Preprocessor.h"
#include "HLSLParser/HLSLParserException.h"
#include "HLSLParser/HLSLTypeVisitor.h"
#include "GLSLSamplerPurger.h"
#include "GLSLEntryPointBuilder.h"
#include "GLSLTextureClass.h"
#include <sstream>
#include <cstring>
#include <algorithm>
#include <cassert>

using namespace std;

GLSLPostIndexGenerator::GLSLPostIndexGenerator(std::ostream& output) :
	mOutput(output)
{

}

void GLSLPostIndexGenerator::Visit(HLSLNode& node)
{
	// do nothing
}

void GLSLPostIndexGenerator::Visit(HLSLNamedTypeNode& node)
{
	node.GetResolvedType()->Accept(*this);
}

void GLSLPostIndexGenerator::Visit(HLSLArrayTypeNode& node)
{
	if (node.GetSize() == HLSLArrayTypeNode::NoSize)
	{
		mOutput << "[]";
	}
	else
	{
		mOutput << '[' << node.GetSize() << ']';
	}
}


////////////////////////////////////////////////

GLSLGenerator::GLSLGenerator(std::ostream& output, ShaderType::Type type, const std::string& entryPoint) :
	mOutput(output),
	mIndentLevel(0),
	mNoSemiColon(false),
	mIsInFunction(false),
	mIsInsideCBuffer(false),
	mShaderType(type),
	mEntryPointName(entryPoint),
	mEntryPoint(NULL)
{

}

GLSLGenerator::~GLSLGenerator(void)
{
}

std::ostream& GLSLGenerator::GetOutput()
{
	return mOutput;
}

void GLSLGenerator::ProcessFile(HLSLNode& file, ShaderType::Type type, const std::string& entryPoint, std::ostream& output)
{
	output << "#version 420" << endl;
	output << "#extension GL_ARB_explicit_attrib_location : require" << endl;

	GLSLSamplerPurger::Purge(file);

	GLSLGenerator generator(output, type, entryPoint);
	file.Accept(generator);

	GLSLEntryPointBuilder entryPointBuilder(generator, type, generator.mEntryPoint);
	entryPointBuilder.GenerateEntryPoint();
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
	mNoSemiColon = false;
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
	mNoSemiColon = true;
}

void GLSLGenerator::Visit(HLSLVariableDefinition& node)
{
	if (!mIsInFunction)
	{
		if (node.GetType().GetType().GetType() == HLSLType::Texture &&
			node.GetRegisterLocation().HasValue())
		{
			mOutput << "layout(binding = " << node.GetRegisterLocation().GetRegisterNumber() << ") ";
		}

		mOutput << "uniform ";
	}

	if (mIsInsideCBuffer)
	{
		mIDRename.AddRename(node.GetName(), node.GetName() + gUniformBufferSuffix[mShaderType]);
	}

	OutputVariableDeclaration(node.GetName(), node.GetType());

	if (node.GetInitialValue() != NULL)
	{
		mOutput << " = ";
		node.GetInitialValue()->Accept(*this);
	}
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
	mOutput << mIDRename.RenameID(node.GetName()) << "(";

	if (node.GetName() == mEntryPointName)
	{
		if (mEntryPoint != NULL)
		{
			throw HLSLParserException(node.GetToken(), "ambigious entry point");
		}
		else
		{
			mEntryPoint = &node;
		}
	}

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
	
	if (node.GetBody() != NULL)
	{
		mIsInFunction = true;
		node.GetBody()->Accept(*this);
		mIsInFunction = false;
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
	mOutput << "struct " << mIDRename.RenameID(node.GetName()) << endl;
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

void GLSLGenerator::Visit(HLSLCBufferDefinition& node)
{
	if (node.GetRegisterLocation().HasValue())
	{
		mOutput << "layout(binding = " << node.GetRegisterLocation().GetRegisterNumber() << ") ";
	}

	mIDRename.AddRename(node.GetName(), node.GetName() + gUniformBufferSuffix[mShaderType]);

	mOutput << "uniform " << mIDRename.RenameID(node.GetName()) << endl;

	OutputIndents();

	mOutput << "{" << endl;

	IncreaseIndent();
	mIsInsideCBuffer = true;

	for (size_t i = 0; i < node.GetMemberCount(); ++i)
	{
		OutputIndents();
		node.GetMember(i).Accept(*this);
		mOutput << ";" << endl;
	}

	mIsInsideCBuffer = false;
	DecreaseIndent();

	OutputIndents();
	mOutput << "}";

	mNoSemiColon = false;
}

void GLSLGenerator::OutputVariableDeclaration(const std::string& variableName, HLSLTypeNode& type)
{
	type.Accept(*this);
	mOutput << " " << mIDRename.RenameID(variableName);
	GLSLPostIndexGenerator indexGenerator(mOutput);
	type.Accept(indexGenerator);
}

void GLSLGenerator::Visit(HLSLVoidNode& node)
{
	mOutput << "void";
}

void GLSLGenerator::Visit(HLSLNamedTypeNode& node)
{
	HLSLTypeNode* type = node.GetResolvedType();
	
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
	GLSLGenerator scalarTypeGenerator(scalarTypeName, mShaderType, mEntryPointName);
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

void GLSLGenerator::Visit(HLSLTextureTypeNode& node)
{
	switch (node.GetToken().GetKeywordType())
	{
	case HLSLKeyword::Texture1D:
		mOutput << "sampler1D";
		break;
	case HLSLKeyword::Texture1DArray:
		mOutput << "sampler1DArray";
		break;
	case HLSLKeyword::Texture2D:
		mOutput << "sampler2D";
		break;
	case HLSLKeyword::Texture2DArray:
		mOutput << "sampler2DArray";
		break;
	case HLSLKeyword::Texture3D:
		mOutput << "sampler2D";
		break;
	case HLSLKeyword::TextureCube:
		mOutput << "samplerCube";
		break;
	default:
		throw HLSLParserException(node.GetToken(), "unsupportted texture type");
	}
}

void GLSLGenerator::Visit(HLSLSamplerTypeNode& node)
{
	throw HLSLParserException(node.GetToken(), "samplers not supported in glsl");
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

	if (node.GetElseBody() != NULL)
	{
		mOutput << endl;
		OutputIndents();
		mOutput << "else";
		node.GetElseBody()->Accept(*this);
	}
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
	if (node.IsTypeName())
	{
		node.GetNamedType()->Accept(*this);
	}
	else
	{
		mOutput << mIDRename.RenameID(node.GetName());
	}
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
	if (node.GetType().GetType() != HLSLType::Scalar && (node.GetToken().IsComparisonOperator() || node.GetToken().IsEqualityOperator()))
	{
		std::string functionName;
		switch (node.GetToken().GetType())
		{
		case HLSLTokenType::LessThan:
			functionName = "lessThan";
			break;
		case HLSLTokenType::LessThanEqual:
			functionName = "lessThanEqual";
			break;
		case HLSLTokenType::GreaterThan:
			functionName = "greaterThan";
			break;
		case HLSLTokenType::GreaterThanEqual:
			functionName = "greaterThanEqual";
			break;
		case HLSLTokenType::Equal:
			functionName = "equal";
			break;
		case HLSLTokenType::NotEqual:
			functionName = "notEqual";
			break;
		default:
			assert(false && "unimplimented comparison operator");
		}

		mOutput << functionName << "(";
		node.GetLeft().Accept(*this);
		mOutput << ", ";
		node.GetRight().Accept(*this);
		mOutput << ")";
	}
	else
	{
		node.GetLeft().Accept(*this);
		mOutput << " " << node.GetToken().GetValue() << " ";
		node.GetRight().Accept(*this);
	}
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
	HLSLStructureNode* structureOperator = dynamic_cast<HLSLStructureNode*>(&node.GetLeft());
	
	bool isFirst = true;

	if (structureOperator != NULL && structureOperator->GetLeft().GetType().GetType() == HLSLType::Texture)
	{
		HLSLType leftType = structureOperator->GetLeft().GetType();
		string functionName = GLSLTextureClass::GetInstance().GetFunctionMapping(leftType.GetTextureType(), structureOperator->GetRight(), node.GetParameterCount());

		if (functionName.length() == 0)
		{
			throw HLSLParserException(node.GetToken(), "undefined function for sampler");
		}
		else
		{
			mOutput << functionName << "(";
			structureOperator->GetLeft().Accept(*this);
			isFirst = false;
		}
	}
	else
	{
		std::ostringstream stringOutput;
		GLSLGenerator nameGenerator(stringOutput, mShaderType, mEntryPointName);
		node.GetLeft().Accept(nameGenerator);

		std::string name = stringOutput.str();

		if (name == "mul" && node.GetParameterCount() == 2 && 
			node.GetParameter(0).GetType().IsNumerical() &&
			node.GetParameter(0).GetType().IsNumerical())
		{
			mOutput << "(";
			node.GetParameter(0).Accept(*this);
			mOutput << " * ";
			node.GetParameter(1).Accept(*this);
			mOutput << ")";
			return;
		}
		else if (name == "saturate" && node.GetParameterCount() == 1)
		{
			mOutput << "clamp(";

			node.GetParameter(0).Accept(*this);

			mOutput << ", 0, 1)";

			return;
		}
		else
		{
			mOutput << name;
			mOutput << '(';
		}
	}

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

const GLSLIDRename& GLSLGenerator::GetIDRename() const
{
	return mIDRename;
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

const char* GLSLGenerator::gUniformBufferSuffix[ShaderType::Count] = {
	"Vert",
	"Hull",
	"Dom",
	"Geo",
	"Pix",
	"Comp"
};