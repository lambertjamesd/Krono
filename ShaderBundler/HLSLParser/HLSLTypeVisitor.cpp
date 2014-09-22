#include "HLSLTypeVisitor.h"
#include "HLSLParserException.h"
#include "HLSLTextureClass.h"
#include <cassert>
#include <cstring>
#include <sstream>
#include "HLSLParser.h"
#include "HLSLBuiltInFunctions.h"

using namespace std;

HLSLTypeStorage::HLSLTypeStorage()
{
	StartScope();
	
	for (size_t i = 1; i <= 4; ++i)
	{
		Define(string("bool") + (char)('0' + i), CreateVectorType(HLSLKeyword::Bool, i));
		Define(string("int") + (char)('0' + i), CreateVectorType(HLSLKeyword::Int, i));
		Define(string("uint") + (char)('0' + i), CreateVectorType(HLSLKeyword::Uint, i));
		Define(string("half") + (char)('0' + i), CreateVectorType(HLSLKeyword::Half, i));
		Define(string("float") + (char)('0' + i), CreateVectorType(HLSLKeyword::Float, i));
		Define(string("double") + (char)('0' + i), CreateVectorType(HLSLKeyword::Double, i));
	}
	
	for (size_t row = 1; row <= 4; ++row)
	{
		for (size_t column = 1; column <= 4; ++column)
		{
			Define(string("bool") + (char)('0' + row) + 'x' + (char)('0' + column), CreateMatrixType(HLSLKeyword::Bool, row, column));
			Define(string("int") + (char)('0' + row) + 'x' + (char)('0' + column), CreateMatrixType(HLSLKeyword::Int, row, column));
			Define(string("uint") + (char)('0' + row) + 'x' + (char)('0' + column), CreateMatrixType(HLSLKeyword::Uint, row, column));
			Define(string("half") + (char)('0' + row) + 'x' + (char)('0' + column), CreateMatrixType(HLSLKeyword::Half, row, column));
			Define(string("float") + (char)('0' + row) + 'x' + (char)('0' + column), CreateMatrixType(HLSLKeyword::Float, row, column));
			Define(string("double") + (char)('0' + row) + 'x' + (char)('0' + column), CreateMatrixType(HLSLKeyword::Double, row, column));
		}
	}
}

void HLSLTypeStorage::StartScope()
{
	mCurrentScope.push_back(map<string,TypeSource>());
}

void HLSLTypeStorage::EndScope()
{
	map<string,TypeSource>& scopeDefines = mCurrentScope.back();

	for (auto it = scopeDefines.begin(); it != scopeDefines.end(); ++it)
	{
		switch (it->second)
		{
		case VariableType:
			mVariableTypes[it->first].pop();
			break;
		case DefineType:
			mDefinedTypes[it->first].pop();
			break;
		}
	}

	mCurrentScope.pop_back();
}

HLSLTypeNode* HLSLTypeStorage::GetDefinedType(const std::string& name)
{
	if (GetTypeSource(name) == DefineType)
	{
		return mDefinedTypes[name].top().get();
	}
	else
	{
		return NULL;
	}
}

std::shared_ptr<HLSLTypeNode> HLSLTypeStorage::GetDefinedTypePtr(const std::string& name)
{
	if (GetTypeSource(name) == DefineType)
	{
		return mDefinedTypes[name].top();
	}
	else
	{
		return NULL;
	}
}

HLSLType HLSLTypeStorage::GetVariableType(const std::string& name)
{
	if (GetTypeSource(name) == VariableType)
	{
		return mVariableTypes[name].top();
	}
	else
	{
		return HLSLType::Void;
	}
}

HLSLType HLSLTypeStorage::GetFunction(const std::string& name)
{
	if (GetTypeSource(name) == FunctionType)
	{
		return HLSLType(*mFunctionStorage[name]);
	}
	else
	{
		return HLSLType::Void;
	}
}

void HLSLTypeStorage::Define(const HLSLToken& startToken, const std::string& name, const std::shared_ptr<HLSLTypeNode>& value)
{
	if (IsDefinedInScope(name))
	{
		throw HLSLParserException(startToken, "redefinition of type");
	}
	else
	{
		mCurrentScope.back()[name] = DefineType;
		mDefinedTypes[name].push(value);
	}
}

void HLSLTypeStorage::DefineVariable(const HLSLToken& startToken, const std::string& name, const HLSLType& type)
{
	if (IsDefinedInScope(name))
	{
		throw HLSLParserException(startToken, "redefinition of type");
	}
	else
	{
		mCurrentScope.back()[name] = VariableType;
		mVariableTypes[name].push(type);
	}
}

void HLSLTypeStorage::DefineFunction(HLSLFunctionDefinition& value)
{
	if (IsDefinedInScope(value.GetName()))
	{
		if (GetTypeSource(value.GetName()) == FunctionType)
		{
			// TODO check for duplicate signatures
			value.SetPreviousOverload(mFunctionStorage[value.GetName()]);
		}
		else
		{
			throw HLSLParserException(value.GetToken(), "redefinition of type");
		}
	}
	else
	{
		mCurrentScope.back()[value.GetName()] = FunctionType;
	}
	
	mFunctionStorage[value.GetName()] = &value;
}

bool HLSLTypeStorage::IsDefinedInScope(const std::string& name)
{
	return mCurrentScope.back().find(name) != mCurrentScope.back().end();
}

HLSLTypeStorage::TypeSource HLSLTypeStorage::GetTypeSource(const std::string& name) const
{
	for (size_t i = mCurrentScope.size() - 1; i < mCurrentScope.size(); --i)
	{
		auto findResult = mCurrentScope[i].find(name);

		if (findResult != mCurrentScope[i].end())
		{
			return findResult->second;
		}
	}

	return None;
}

void HLSLTypeStorage::Define(const std::string& name, const std::shared_ptr<HLSLTypeNode>& value)
{
	Define(HLSLToken(HLSLTokenType::Identifier, name, 0), name, value);
}

std::unique_ptr<HLSLScalarTypeNode> HLSLTypeStorage::CreateScalarType(HLSLKeyword::Type scalarType)
{
	std::unique_ptr<HLSLScalarTypeNode> result;

	switch (scalarType)
	{
	case HLSLKeyword::Bool:
		result.reset(new HLSLScalarTypeNode(HLSLToken(HLSLTokenType::Identifier, "bool", 0)));
		result->ResolveType(HLSLType::Bool);
		break;
	case HLSLKeyword::Int:
		result.reset(new HLSLScalarTypeNode(HLSLToken(HLSLTokenType::Identifier, "int", 0)));
		result->ResolveType(HLSLType::Int);
		break;
	case HLSLKeyword::Uint:
		result.reset(new HLSLScalarTypeNode(HLSLToken(HLSLTokenType::Identifier, "uint", 0)));
		result->ResolveType(HLSLType::UInt);
		break;
	case HLSLKeyword::Dword:
		result.reset(new HLSLScalarTypeNode(HLSLToken(HLSLTokenType::Identifier, "dword", 0)));
		result->ResolveType(HLSLType::DWord);
		break;
	case HLSLKeyword::Half:
		result.reset(new HLSLScalarTypeNode(HLSLToken(HLSLTokenType::Identifier, "half", 0)));
		result->ResolveType(HLSLType::Half);
		break;
	case HLSLKeyword::Float:
		result.reset(new HLSLScalarTypeNode(HLSLToken(HLSLTokenType::Identifier, "float", 0)));
		result->ResolveType(HLSLType::Float);
		break;
	case HLSLKeyword::Double:
		result.reset(new HLSLScalarTypeNode(HLSLToken(HLSLTokenType::Identifier, "double", 0)));
		result->ResolveType(HLSLType::Double);
		break;
	default:
		throw HLSLParserException("Invalid scalar type");
	}

	return move(result);
}

std::shared_ptr<HLSLTypeNode> HLSLTypeStorage::CreateVectorType(HLSLKeyword::Type scalarType, size_t size)
{
	std::unique_ptr<HLSLScalarTypeNode> scalarNode(move(CreateScalarType(scalarType)));
	HLSLType vectorType = scalarNode->GetType().VectorType(size);
	std::shared_ptr<HLSLVectorTypeNode> result(new HLSLVectorTypeNode(HLSLToken(HLSLTokenType::Identifier, "vector", 0), move(scalarNode), size));
	result->ResolveType(vectorType);
	return move(result);
}

std::shared_ptr<HLSLTypeNode> HLSLTypeStorage::CreateMatrixType(HLSLKeyword::Type scalarType, size_t rows, size_t columns)
{
	std::unique_ptr<HLSLScalarTypeNode> scalarNode(move(CreateScalarType(scalarType)));
	HLSLType matrixType = scalarNode->GetType().MatrixType(rows, columns);
	std::shared_ptr<HLSLMatrixTypeNode> result(new HLSLMatrixTypeNode(HLSLToken(HLSLTokenType::Identifier, "vector", 0), move(scalarNode), rows, columns));
	result->ResolveType(matrixType);
	return move(result);
}

HLSLTypeVisitor::HLSLTypeVisitor(void) :
	mCurrentFunction(NULL)
{

}


HLSLTypeVisitor::~HLSLTypeVisitor(void)
{
}

void HLSLTypeVisitor::LoadBuiltInFunctions()
{
	HLSLBuiltInFunctions::PopulateVisitor(*this);
}

void HLSLTypeVisitor::DefineFunction(HLSLFunctionDefinition& value)
{
	mTypeStorage.DefineFunction(value);
}
	
void HLSLTypeVisitor::Visit(HLSLStatementBlock& node)
{
	mTypeStorage.StartScope();

	HLSLNodeVisitor::Visit(node);

	mTypeStorage.EndScope();
}

void HLSLTypeVisitor::Visit(HLSLTypedefDefinition& node)
{
	node.GetType()->Accept(*this);
	mTypeStorage.Define(node.GetToken(), node.GetName(), node.GetType());
}

void HLSLTypeVisitor::Visit(HLSLVariableDefinition& node)
{
	node.GetType().Accept(*this);

	if (node.GetType().GetType().GetType() == HLSLType::Void)
	{
		throw HLSLParserException(node.GetToken(), "cannot have variable of type void");
	}

	mTypeStorage.DefineVariable(node.GetToken(), node.GetName(), node.GetType().GetType());
	
	if (node.GetInitialValue() != NULL)
	{
		node.GetInitialValue()->Accept(*this);
	}
}

void HLSLTypeVisitor::Visit(HLSLFunctionParameter& node)
{
	node.GetType().Accept(*this);

	if (node.GetType().GetType().GetType() == HLSLType::Void)
	{
		throw HLSLParserException(node.GetToken(), "cannot have variable of type void");
	}

	mTypeStorage.DefineVariable(node.GetToken(), node.GetName(), node.GetType().GetType());

	if (node.GetInitializer() != NULL)
	{
		node.GetInitializer()->Accept(*this);
	}
}

void HLSLTypeVisitor::Visit(HLSLFunctionDefinition& node)
{
	mTypeStorage.DefineFunction(node);

	mTypeStorage.StartScope();

	node.GetReturnType().Accept(*this);

	for (size_t i = 0; i < node.GetParameterCount(); ++i)
	{
		node.GetParameter(i).Accept(*this);
	}

	mCurrentFunction = &node;

	if (node.GetBody() != NULL)
	{
		node.GetBody()->Accept(*this);
	}

	mCurrentFunction = NULL;

	mTypeStorage.EndScope();
}

void HLSLTypeVisitor::Visit(HLSLReturnStatement& node)
{
	node.GetReturnValue().Accept(*this);

	if (!mCurrentFunction->GetReturnType().GetType().CanAssignFrom(node.GetReturnValue().GetType()))
	{
		throw HLSLParserException(node.GetToken(), "invalid return type");
	}
}

void HLSLTypeVisitor::Visit(HLSLStructureMember& node)
{
	node.GetType().Accept(*this);
}

void HLSLTypeVisitor::Visit(HLSLStructDefinition& node)
{
	mTypeStorage.StartScope();

	for (size_t i = 0; i < node.GetMemberCount(); ++i)
	{
		node.GetMember(i).Accept(*this);
	}

	mTypeStorage.EndScope();

	mTypeStorage.Define(node.GetToken(), node.GetName(), shared_ptr<HLSLStructTypeNode>(new HLSLStructTypeNode(node)));
}

void HLSLTypeVisitor::Visit(HLSLCBufferDefinition& node)
{
	// cbuffer declare variables that live in the global
	// scope, so no start end scope needed
	for (size_t i = 0; i < node.GetMemberCount(); ++i)
	{
		node.GetMember(i).Accept(*this);
	}
}

void HLSLTypeVisitor::Visit(HLSLVoidNode& node)
{
	node.ResolveType(HLSLType::Void);
}

void HLSLTypeVisitor::Visit(HLSLNamedTypeNode& node)
{
	std::shared_ptr<HLSLTypeNode> typeNode = mTypeStorage.GetDefinedTypePtr(node.GetName());

	if (typeNode == NULL)
	{
		throw HLSLParserException(node.GetToken(), "undefined type");
	}
	else
	{
		if (typeNode->GetType().GetType() == HLSLType::Unknown)
		{
			typeNode->Accept(*this);
		}

		node.ResolveType(typeNode);
		node.ResolveType(typeNode->GetType());
	}
}

void HLSLTypeVisitor::Visit(HLSLScalarTypeNode& node)
{
	switch (node.GetToken().GetKeywordType())
	{
	case HLSLKeyword::Bool:
		node.ResolveType(HLSLType::Bool);
		break;
	case HLSLKeyword::Int:
		node.ResolveType(HLSLType::Int);
		break;
	case HLSLKeyword::Uint:
		node.ResolveType(HLSLType::UInt);
		break;
	case HLSLKeyword::Dword:
		node.ResolveType(HLSLType::DWord);
		break;
	case HLSLKeyword::Half:
		node.ResolveType(HLSLType::Half);
		break;
	case HLSLKeyword::Float:
		node.ResolveType(HLSLType::Float);
		break;
	case HLSLKeyword::Double:
		node.ResolveType(HLSLType::Double);
		break;
	case HLSLKeyword::__bool_int_float__:
		node.ResolveType(HLSLType::BoolIntFloat);
		break;
	case HLSLKeyword::__int_float__:
		node.ResolveType(HLSLType::IntFloat);
		break;
	default:
		throw HLSLParserException(node.GetToken(), "scalar type not supported");
	}
}

void HLSLTypeVisitor::Visit(HLSLBufferTypeNode& node)
{
	throw HLSLParserException(node.GetToken(), "buffer type not currently supported");
}

void HLSLTypeVisitor::Visit(HLSLArrayTypeNode& node)
{
	node.GetInternalType().Accept(*this);
	HLSLType internalType = node.GetInternalType().GetType();

	if (internalType.IsArray())
	{
		throw HLSLParserException(node.GetToken(), "2D array not allowed");
	}
	else
	{
		node.ResolveType(internalType.ArrayType(node.GetSize()));
	}
}

void HLSLTypeVisitor::Visit(HLSLVectorTypeNode& node)
{
	node.GetInternalType().Accept(*this);
	HLSLType internalType = node.GetInternalType().GetType();

	if (internalType.GetType() == HLSLType::Scalar)
	{
		node.ResolveType(HLSLType(internalType.GetScalarType(), node.GetSize()));
	}
	else
	{
		throw HLSLParserException(node.GetInternalType().GetToken(), "vector must be of scalar type");
	}
}

void HLSLTypeVisitor::Visit(HLSLMatrixTypeNode& node)
{
	node.GetInternalType().Accept(*this);
	HLSLType internalType = node.GetInternalType().GetType();

	if (internalType.GetType() == HLSLType::Scalar)
	{
		node.ResolveType(HLSLType(internalType.GetScalarType(), node.GetRows(), node.GetColumns()));
	}
	else
	{
		throw HLSLParserException(node.GetInternalType().GetToken(), "matrix must be of scalar type");
	}
}

void HLSLTypeVisitor::Visit(HLSLNumericalTypeNode& node)
{
	node.GetInternalType().Accept(*this);
	HLSLType internalType = node.GetInternalType().GetType();

	if (internalType.GetType() == HLSLType::Scalar)
	{
		node.ResolveType(HLSLType(HLSLType::Numerical, internalType.GetScalarType()));
	}
	else
	{
		throw HLSLParserException(node.GetInternalType().GetToken(), "numerical must be of scalar type");
	}
}

void HLSLTypeVisitor::Visit(HLSLVariableVectorTypeNode& node)
{
	node.GetInternalType().Accept(*this);
	HLSLType internalType = node.GetInternalType().GetType();

	if (internalType.GetType() == HLSLType::Scalar)
	{
		node.ResolveType(HLSLType(HLSLType::VariableVector, internalType.GetScalarType()));
	}
	else
	{
		throw HLSLParserException(node.GetInternalType().GetToken(), "variable vector must be of scalar type");
	}
}

void HLSLTypeVisitor::Visit(HLSLTextureTypeNode& node)
{
	switch (node.GetToken().GetKeywordType())
	{
	case HLSLKeyword::Texture1D:
		node.ResolveType(HLSLType::Texture1D);
		break;
	case HLSLKeyword::Texture1DArray:
		node.ResolveType(HLSLType::Texture1DArray);
		break;
	case HLSLKeyword::Texture2D:
		node.ResolveType(HLSLType::Texture2D);
		break;
	case HLSLKeyword::Texture2DArray:
		node.ResolveType(HLSLType::Texture2DArray);
		break;
	case HLSLKeyword::Texture3D:
		node.ResolveType(HLSLType::Texture3D);
		break;
	case HLSLKeyword::TextureCube:
		node.ResolveType(HLSLType::TextureCube);
		break;
	default:
		throw HLSLParserException(node.GetToken(), "unsupported texture type");
	}
}

void HLSLTypeVisitor::Visit(HLSLSamplerTypeNode& node)
{
	node.ResolveType(HLSLType(HLSLType::Sampler));
}

void HLSLTypeVisitor::Visit(HLSLStructTypeNode& node)
{
	node.ResolveType(HLSLType(node.GetDefinition()));
}

void HLSLTypeVisitor::Visit(HLSLFunctionTypeNode& node)
{

}

void HLSLTypeVisitor::CheckBooleanExpression(HLSLExpressionNode& node)
{
	HLSLType conditionType = node.GetType();

	if (!conditionType.IsSingleValue() || !HLSLType(HLSLType::Bool).CanAssignFrom(conditionType))
	{
		throw HLSLParserException(node.GetToken(), "expression does not evaluate to bool");
	}
}

void HLSLTypeVisitor::Visit(HLSLDoNode& node)
{
	node.GetBody().Accept(*this);
	node.GetCondition().Accept(*this);
	CheckBooleanExpression(node.GetCondition());
}

void HLSLTypeVisitor::Visit(HLSLIfNode& node)
{
	node.GetCondition().Accept(*this);
	CheckBooleanExpression(node.GetCondition());
	node.GetBody().Accept(*this);

	if (node.GetElseBody() != NULL)
	{
		node.GetElseBody()->Accept(*this);
	}
}

void HLSLTypeVisitor::Visit(HLSLForNode& node)
{
	node.GetInitializer().Accept(*this);
	node.GetCondition().Accept(*this);
	node.GetModifier().Accept(*this);
	CheckBooleanExpression(node.GetCondition());
	node.GetBody().Accept(*this);
}

void HLSLTypeVisitor::Visit(HLSLSwitchNode& node)
{
	node.GetSelectValue().Accept(*this);
	CheckBooleanExpression(node.GetSelectValue());
	node.GetBody().Accept(*this);
}

void HLSLTypeVisitor::Visit(HLSLSwitchLabelNode& node)
{
	// do nothing
}

void HLSLTypeVisitor::Visit(HLSLDefaultNode& node)
{
	// do nothing
}

void HLSLTypeVisitor::Visit(HLSLWhileNode& node)
{
	node.GetCondition().Accept(*this);
	CheckBooleanExpression(node.GetCondition());
	node.GetBody().Accept(*this);
}

void HLSLTypeVisitor::Visit(HLSLBreakNode& node)
{
	// Do Nothing
}

void HLSLTypeVisitor::Visit(HLSLContinueNode& node)
{
	// Do Nothing
}

void HLSLTypeVisitor::Visit(HLSLDiscardNode& node)
{
	// Do Nothing
}

void HLSLTypeVisitor::Visit(HLSLIdentifierNode& node)
{
	HLSLTypeStorage::TypeSource type = mTypeStorage.GetTypeSource(node.GetName());

	if (type == HLSLTypeStorage::FunctionType)
	{
		HLSLType variableType = mTypeStorage.GetFunction(node.GetName());
		node.ResolveType(variableType);
	}
	else if (type == HLSLTypeStorage::VariableType)
	{
		HLSLType variableType = mTypeStorage.GetVariableType(node.GetName());
		node.ResolveType(variableType);
	}
	else if (type == HLSLTypeStorage::DefineType)
	{
		node.ResolveNamedType(mTypeStorage.GetDefinedTypePtr(node.GetName()));
		node.ResolveType(node.GetNamedType()->GetType().TypeClass());
	}
	else
	{
		throw HLSLParserException(node.GetToken(), "identifier does not name a type");
	}
}

void HLSLTypeVisitor::Visit(HLSLNumberConstantNode& node)
{
	HLSLType::ScalarType scalarType;

	const string& nodeValue = node.GetStringValue();

	if (nodeValue.find('.') == string::npos)
	{
		if (tolower(nodeValue.back()) == 'u')
		{
			scalarType = HLSLType::UInt;
		}
		else
		{
			scalarType = HLSLType::Int;
		}
	}
	else
	{
		if (tolower(nodeValue.back()) == 'h')
		{
			scalarType = HLSLType::Half;
		}
		else if (tolower(nodeValue.back()) == 'l')
		{
			scalarType = HLSLType::Double;
		}
		else
		{
			scalarType = HLSLType::Float;
		}
	}

	node.ResolveType(HLSLType(scalarType));
}

void HLSLTypeVisitor::Visit(HLSLBooleanConstantNode& node)
{
	node.ResolveType(HLSLType(HLSLType::Bool));
}

void HLSLTypeVisitor::Visit(HLSLTypeExpressionNode& node)
{
	node.GetType().Accept(*this);
	node.ResolveType(node.GetType().GetType().TypeClass());
}

void HLSLTypeVisitor::Visit(HLSLParenthesisNode& node)
{
	node.GetValue().Accept(*this);
	node.ResolveType(node.GetValue().GetType());
}
	
void HLSLTypeVisitor::Visit(HLSLBinaryOperatorNode& node)
{
	node.GetLeft().Accept(*this);
	node.GetRight().Accept(*this);

	HLSLType leftType = node.GetLeft().GetType();
	HLSLType rightType = node.GetRight().GetType();

	const HLSLToken token = node.GetToken();

	if (token.GetType() == HLSLTokenType::Assign)
	{
		if (leftType.CanAssignFrom(rightType))
		{
			node.ResolveType(leftType);
		}
		else
		{
			throw HLSLParserException(token, "cannot assign right into left");
		}
	}
	else if (token.IsEqualityOperator())
	{
		if (leftType.GetType() == HLSLType::Sampler ||
			rightType.GetType() == HLSLType::Sampler)
		{
			throw HLSLParserException(token, "cannot compare samplers");
		}

		if (leftType.CanAssignFrom(rightType))
		{
			node.ResolveType(leftType.BoolType());
		}
		else if (rightType.CanAssignFrom(leftType))
		{
			node.ResolveType(rightType.BoolType());
		}
		else
		{
			throw HLSLParserException(token, "no equality operator defined");
		}
	}
	else if (leftType.IsNumerical() && rightType.IsNumerical())
	{
		if (leftType.IsArray() || rightType.IsArray())
		{
			throw HLSLParserException(token, "cannot perform arithmetic on array");
		}
		else if (leftType.IsTypeClass() || rightType.IsTypeClass())
		{
			throw HLSLParserException(token, "cannot perform arithmetic on type");
		}

		if (token.IsComparisonOperator())
		{
			if (leftType.CanAssignFrom(rightType))
			{
				node.ResolveType(leftType.BoolType());
			}
			else if (rightType.CanAssignFrom(leftType))
			{
				node.ResolveType(rightType.BoolType());
			}
			else
			{
				throw HLSLParserException(token, "no comparison operator defined");
			}
		}
		else
		{
			if (token.IsBitwiseOperator() && (!leftType.IsInteger() || rightType.IsInteger()))
			{
				throw HLSLParserException(token, "integer type required");
			}

			HLSLType::ScalarType scalarType = HLSLType::MorePowerfulScalar(leftType.GetScalarType(), rightType.GetScalarType());

			if (leftType.IsSingleValue() && !rightType.IsSingleValue())
			{
				node.ResolveType(HLSLType(scalarType));
			}
			else if (rightType.IsSingleValue() && !leftType.IsSingleValue())
			{
				node.ResolveType(leftType.ChangeScalarType(scalarType));
			}
			else if (leftType.CanAssignFrom(rightType))
			{
				node.ResolveType(leftType.ChangeScalarType(scalarType));
			}
			else if (rightType.CanAssignFrom(leftType))
			{
				node.ResolveType(rightType.ChangeScalarType(scalarType));
			}
			else
			{
				throw HLSLParserException(token, "no operator defined");
			}
		}
	}
	else
	{
		throw HLSLParserException(token, "no operator defined");
	}
}
	
void HLSLTypeVisitor::Visit(HLSLUnaryOperator& node)
{
	node.GetInnerExpression().Accept(*this);
	HLSLType innerType = node.GetInnerExpression().GetType();

	if (innerType.IsNumerical() && (!innerType.IsArray() && !innerType.IsTypeClass()))
	{
		if (node.GetToken().GetType() == HLSLTokenType::BooleanNot)
		{
			node.ResolveType(HLSLType(HLSLType::Bool, innerType.GetVectorSize()));
		}
		else
		{
			node.ResolveType(innerType);
		}
	}
	else
	{
		throw HLSLParserException(node.GetToken(), "cannot negate value");
	}
}

void HLSLTypeVisitor::Visit(HLSLCastNode& node)
{
	node.GetRight().Accept(*this);
	node.GetType().Accept(*this);

	HLSLType sourceType = node.GetRight().GetType();
	HLSLType castTo = node.GetType().GetType();

	node.ResolveType(castTo);
}

void HLSLTypeVisitor::Visit(HLSLPrefixIncrNode& node)
{
	node.GetRight().Accept(*this);
	node.ResolveType(node.GetRight().GetType());
}

void HLSLTypeVisitor::Visit(HLSLPrefixDecrNode& node)
{
	node.GetRight().Accept(*this);
	node.ResolveType(node.GetRight().GetType());
}

void HLSLTypeVisitor::Visit(HLSLIndexNode& node)
{
	node.GetLeft().Accept(*this);
	node.GetIndex().Accept(*this);

	HLSLType indexType = node.GetIndex().GetType();

	if (!indexType.IsSingleValue() || !indexType.IsInteger())
	{
		throw HLSLParserException(node.GetIndex().GetToken(), "array index must be an integer");
	}

	HLSLType arrayType = node.GetLeft().GetType();

	if (!arrayType.IsIndexable())
	{
		throw HLSLParserException(node.GetIndex().GetToken(), "cannot index non array type");
	}

	node.ResolveType(arrayType.ArrayElementType());
}

void HLSLTypeVisitor::Visit(HLSLPostfixIncr& node)
{
	node.GetLeft().Accept(*this);
	node.ResolveType(node.GetLeft().GetType());
}

void HLSLTypeVisitor::Visit(HLSLPostfixDecr& node)
{
	node.GetLeft().Accept(*this);
	node.ResolveType(node.GetLeft().GetType());
}

void HLSLTypeVisitor::Visit(HLSLStructureNode& node)
{
	node.GetLeft().Accept(*this);

	HLSLType structureType = node.GetLeft().GetType();
	
	switch (structureType.GetType())
	{
	case HLSLType::Struct:
	{
		HLSLType resolvedType = structureType.SubElement(node.GetRight());

		if (resolvedType.GetType() == HLSLType::Unknown)
		{
			throw HLSLParserException(node.GetToken(), "not a member of structure");
		}
		else
		{
			node.ResolveType(resolvedType);
		}
		break;
	}
	case HLSLType::Scalar:
		{
			HLSLType type = ParseVectorSwizzle(structureType.GetScalarType(), node.GetRight(), 1);

			if (type.GetType() == HLSLType::Unknown)
			{
				throw HLSLParserException(node.GetToken(), "invalid swizzle value");
			}
			else
			{
				node.ResolveType(type);
			}
		}
		break;
	case HLSLType::Vector:
		{
			HLSLType type = ParseVectorSwizzle(structureType.GetScalarType(), node.GetRight(), structureType.GetVectorSize());

			if (type.GetType() == HLSLType::Unknown)
			{
				throw HLSLParserException(node.GetToken(), "invalid swizzle value");
			}
			else
			{
				node.ResolveType(type);
			}
		}
		break;
	case HLSLType::Matrix:
		{

			HLSLType type = ParseMatrixSwizzle(structureType.GetScalarType(), node.GetRight(), structureType.GetRows(), structureType.GetColumns());

			if (type.GetType() == HLSLType::Unknown)
			{
				throw HLSLParserException(node.GetToken(), "invalid swizzle value");
			}
			else
			{
				node.ResolveType(type);
			}
		} 
		break;
	case HLSLType::Texture:
		{
			HLSLType resolvedType = HLSLTextureClass::GetInstance().GetMemberType(structureType.GetTextureType(), node.GetRight());
			
			if (resolvedType.GetType() == HLSLType::Unknown)
			{
				throw HLSLParserException(node.GetToken(), "not a member of texture");
			}
			else
			{
				node.ResolveType(resolvedType);
			}
		}
		break;
	default:
		throw HLSLParserException(node.GetToken(), "Not yet implemented");
		break;
	}
}

HLSLFunctionDefinition& HLSLTypeVisitor::ResolveFunctionOverload(const HLSLToken& functionCallToken, const std::string& functionName, const HLSLType& functionType, const HLSLFunctionInputSignature& inputSignature) const
{
	assert(functionType.GetType() == HLSLType::Function);

	HLSLFunctionDefinition* exactMatch = NULL;
	HLSLFunctionDefinition* compatableMatch = NULL;
	bool isAbigious = false;

	HLSLFunctionDefinition* looper = &functionType.GetFunction();

	while (looper != NULL)
	{
		if (looper->Matches(inputSignature))
		{
			if (exactMatch != NULL)
			{
				throw HLSLParserException(functionCallToken, "Ambigious function call " + functionName);
			}
			else
			{
				exactMatch = looper;
			}
		}
		else if (looper->IsCompatibleWith(inputSignature))
		{
			if (compatableMatch != NULL)
			{
				isAbigious = true;
			}
			else
			{
				compatableMatch = looper;
			}
		}

		looper = looper->GetPreviousOverload();
	}


	if (exactMatch != NULL)
	{
		functionType.ResolveFunctionOverload(*exactMatch);
		return *exactMatch;
	}
	else if (compatableMatch != NULL)
	{
		if (isAbigious)
		{
			throw HLSLParserException(functionCallToken, "Ambigious function call " + functionName);
		}
		else
		{
			functionType.ResolveFunctionOverload(*compatableMatch);
			return *compatableMatch;
		}
	}
	else
	{
		throw HLSLParserException(functionCallToken, "No overloaded function matches parameters for function named " + functionName);
	}
}

void HLSLTypeVisitor::Visit(HLSLFunctionCallNode& node)
{
	node.GetLeft().Accept(*this);

	vector<HLSLType> types;

	for (size_t i  = 0; i < node.GetParameterCount(); ++i)
	{
		node.GetParameter(i).Accept(*this);
		types.push_back(node.GetParameter(i).GetType());
	}

	HLSLType leftType = node.GetLeft().GetType();

	if (leftType.GetType() == HLSLType::Function)
	{
		std::string name = leftType.GetFunction().GetName();

		if (name == "mul" && node.GetParameterCount() == 2)
		{
			int leftRows, leftColumns;
			int rightRows, rightColumns;

			if (CanBeMatrix(types[0], leftRows, leftColumns, true) && CanBeMatrix(types[1], rightRows, rightColumns, false))
			{
				if (leftColumns == rightRows)
				{
					if (types[0].GetType() == HLSLType::Vector)
					{
						node.ResolveType(HLSLType(types[0].GetScalarType(), rightColumns));
					}
					else if (types[1].GetType() == HLSLType::Vector)
					{
						node.ResolveType(HLSLType(types[0].GetScalarType(), leftRows));
					}
					else
					{
						node.ResolveType(HLSLType(types[0].GetScalarType(), leftRows, rightColumns));
					}
				}
				else
				{
					throw HLSLParserException(node.GetToken(), "row and column mismatch");
				}
			}
			else
			{
				throw HLSLParserException(node.GetToken(), "cannot multiply non numerical types");
			}
		}
		else
		{
			HLSLFunctionInputSignature inputSignature(types);
			HLSLFunctionDefinition& functionDefinition = ResolveFunctionOverload(node.GetToken(), name, node.GetLeft().GetType(), inputSignature);
			HLSLType returnType = functionDefinition.GetReturnType().GetType();

			if (!functionDefinition.ResolveReturnType(inputSignature, returnType))
			{
				throw HLSLParserException(node.GetToken(), "Could not resolve return type from parameters");
			}

			node.ResolveType(returnType);
		}
	}
	else if (leftType.IsTypeClass())
	{
		if (leftType.IsNumerical())
		{
			// TODO check parameter counts
			node.ResolveType(leftType.InstanceType());
		}
		else
		{
			throw HLSLParserException(node.GetToken(), "Unsupported constructor");
		}
	}
	else
	{
		throw HLSLParserException(node.GetToken(), "not a function");
	}
}

void HLSLTypeVisitor::SetBuiltInDefinitions(std::unique_ptr<HLSLNode> definitionsRoot)
{
	assert(mDefinitionsRoot == NULL && "definitions can only be set once");
	mDefinitionsRoot = move(definitionsRoot);
	mDefinitionsRoot->Accept(*this);
}

bool HLSLTypeVisitor::CanBeMatrix(const HLSLType& type, int& rows, int& columns, bool columnVector)
{
	switch (type.GetType())
	{
	case HLSLType::Scalar:
		rows = 1;
		columns = 1;
		return true;
	case HLSLType::Vector:
		rows = columnVector ? 1 : type.GetVectorSize();
		columns = columnVector ? type.GetVectorSize() : 1;
		return true;
	case HLSLType::Matrix:
		rows = type.GetRows();
		columns = type.GetColumns();
		return true;
	}

	return false;
}

bool HLSLTypeVisitor::IsVectorSwizzle(char character, size_t vectorSize)
{
	std::string vectorSwizzles("xyzw");
	return vectorSwizzles.find(character) <= (vectorSize - 1);
}

bool HLSLTypeVisitor::IsColorSwizzle(char character, size_t vectorSize)
{
	std::string vectorSwizzles("rgba");
	return vectorSwizzles.find(character) <= (vectorSize - 1);
}

bool HLSLTypeVisitor::IsZeroIndexSwizzle(const char* str, size_t rows, size_t columns)
{
	return str[0] == '_' &&
		str[1] == 'm' &&
		(size_t)(str[2] - '0') < (rows - 1) &&
		(size_t)(str[3] - '0') < (columns - 1);
}

bool HLSLTypeVisitor::IsOneIndexSwizzle(const char* str, size_t rows, size_t columns)
{
	return str[0] == '_' &&
		(size_t)(str[1] - '1') < (rows - 1) &&
		(size_t)(str[2] - '1') < (columns - 1);
}

HLSLType HLSLTypeVisitor::ParseVectorSwizzle(HLSLType::ScalarType scalar, const std::string& value, size_t vectorSize)
{
	if (value.length() > 4)
	{
		return HLSLType();
	}

	if (IsVectorSwizzle(value[0], vectorSize))
	{
		for (size_t i = 1; i < value.length(); ++i)
		{
			if (!IsVectorSwizzle(value[i], vectorSize))
			{
				return HLSLType();
			}
		}

		return HLSLType(scalar, value.length());
	}

	if (IsColorSwizzle(value[0], vectorSize))
	{
		for (size_t i = 1; i < value.length(); ++i)
		{
			if (!IsColorSwizzle(value[i], vectorSize))
			{
				return HLSLType();
			}
		}

		return HLSLType(scalar, value.length());
	}

	return HLSLType();
}

HLSLType HLSLTypeVisitor::ParseMatrixSwizzle(HLSLType::ScalarType scalar, const std::string& value, size_t rows, size_t columns)
{
	const char* valueStr = value.c_str();
	size_t currentPosition = 0;

	if (IsZeroIndexSwizzle(valueStr, rows, columns))
	{
		static const char* ZeroIndexElement = "_m00";

		currentPosition += strlen(ZeroIndexElement);

		while (IsZeroIndexSwizzle(valueStr + currentPosition, rows, columns))
		{
			currentPosition += strlen(ZeroIndexElement);
		}

		size_t count = currentPosition / strlen(ZeroIndexElement);

		if (currentPosition == value.length() && count <= 4)
		{
			return HLSLType(scalar, count);
		}
	}
	else if (IsOneIndexSwizzle(valueStr, rows, columns))
	{
		static const char* ZeroIndexElement = "_00";

		currentPosition += strlen(ZeroIndexElement);

		while (IsOneIndexSwizzle(valueStr + currentPosition, rows, columns))
		{
			currentPosition += strlen(ZeroIndexElement);
		}

		size_t count = currentPosition / strlen(ZeroIndexElement);

		if (currentPosition == value.length() && count <= 4)
		{
			return HLSLType(scalar, count);
		}
	}
	
	return HLSLType();
}
