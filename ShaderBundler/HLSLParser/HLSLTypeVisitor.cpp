#include "HLSLTypeVisitor.h"
#include "HLSLParserException.h"
#include <cassert>

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
	if (GetTypeSource(name) == DefineType)
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
	for (auto it = mCurrentScope.rbegin(); it != mCurrentScope.rend(); ++it)
	{
		auto findResult = it->find(name);

		if (findResult != it->end())
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
	switch (scalarType)
	{
	case HLSLKeyword::Bool:
		return std::unique_ptr<HLSLScalarTypeNode>(new HLSLScalarTypeNode(HLSLToken(HLSLTokenType::Identifier, "bool", 0)));
	case HLSLKeyword::Int:
		return std::unique_ptr<HLSLScalarTypeNode>(new HLSLScalarTypeNode(HLSLToken(HLSLTokenType::Identifier, "int", 0)));
	case HLSLKeyword::Uint:
		return std::unique_ptr<HLSLScalarTypeNode>(new HLSLScalarTypeNode(HLSLToken(HLSLTokenType::Identifier, "uint", 0)));
	case HLSLKeyword::Dword:
		return std::unique_ptr<HLSLScalarTypeNode>(new HLSLScalarTypeNode(HLSLToken(HLSLTokenType::Identifier, "dword", 0)));
	case HLSLKeyword::Half:
		return std::unique_ptr<HLSLScalarTypeNode>(new HLSLScalarTypeNode(HLSLToken(HLSLTokenType::Identifier, "half", 0)));
	case HLSLKeyword::Float:
		return std::unique_ptr<HLSLScalarTypeNode>(new HLSLScalarTypeNode(HLSLToken(HLSLTokenType::Identifier, "float", 0)));
	case HLSLKeyword::Double:
		return std::unique_ptr<HLSLScalarTypeNode>(new HLSLScalarTypeNode(HLSLToken(HLSLTokenType::Identifier, "double", 0)));
	default:
		throw HLSLParserException("Invalid scalar type");
	}
}

std::shared_ptr<HLSLTypeNode> HLSLTypeStorage::CreateVectorType(HLSLKeyword::Type scalarType, size_t size)
{
	std::unique_ptr<HLSLScalarTypeNode> scalarNode(move(CreateScalarType(scalarType)));
	return std::shared_ptr<HLSLVectorTypeNode>(new HLSLVectorTypeNode(HLSLToken(HLSLTokenType::Identifier, "vector", 0), move(scalarNode), size));
}

std::shared_ptr<HLSLTypeNode> HLSLTypeStorage::CreateMatrixType(HLSLKeyword::Type scalarType, size_t rows, size_t columns)
{
	std::unique_ptr<HLSLScalarTypeNode> scalarNode(move(CreateScalarType(scalarType)));
	return std::shared_ptr<HLSLMatrixTypeNode>(new HLSLMatrixTypeNode(HLSLToken(HLSLTokenType::Identifier, "vector", 0), move(scalarNode), rows, columns));
}

HLSLTypeVisitor::HLSLTypeVisitor(void)
{
}


HLSLTypeVisitor::~HLSLTypeVisitor(void)
{
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
	mTypeStorage.DefineVariable(node.GetToken(), node.GetName(), node.GetType().GetType());
	
	if (node.GetInitialValue() != NULL)
	{
		node.GetInitialValue()->Accept(*this);
	}
}

void HLSLTypeVisitor::Visit(HLSLFunctionParameter& node)
{
	node.GetType().Accept(*this);
	mTypeStorage.DefineVariable(node.GetToken(), node.GetName(), node.GetType().GetType());

	if (node.GetInitializer() != NULL)
	{
		node.GetInitializer()->Accept(*this);
	}
}

void HLSLTypeVisitor::Visit(HLSLFunctionDefinition& node)
{
	mTypeStorage.StartScope();

	node.GetReturnType().Accept(*this);

	for (size_t i = 0; i < node.GetParameterCount(); ++i)
	{
		node.GetParameter(i).Accept(*this);
	}

	mTypeStorage.DefineFunction(node);

	if (node.GetBody() != NULL)
	{
		node.GetBody()->Accept(*this);
	}

	mTypeStorage.EndScope();
}

void HLSLTypeVisitor::Visit(HLSLReturnStatement& node)
{
	node.GetReturnValue().Accept(*this);

	// TODO check function return value
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
		typeNode->Accept(*this);
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
		HLSLType classType = mTypeStorage.GetDefinedType(node.GetName())->GetType().TypeClass();
		node.ResolveType(classType);
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

	// TODO, determine compatibility
	// derive return type
}
	
void HLSLTypeVisitor::Visit(HLSLUnaryOperator& node)
{
	node.GetInnerExpression().Accept(*this);
	HLSLType innerType = node.GetInnerExpression().GetType();

	if (node.GetToken().GetType() == HLSLTokenType::BooleanNot)
	{
		if (innerType.GetType() == HLSLType::Vector)
		{
			node.ResolveType(HLSLType(HLSLType::Bool, innerType.GetVectorSize()));
		}
		else
		{
			throw HLSLParserException(node.GetToken(), "cannot negate value");
		}
	}
	else
	{
		node.ResolveType(innerType);
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

	if (indexType.GetType() != HLSLType::Vector ||
		!(indexType.GetType() == HLSLType::UInt ||
		indexType.GetType() == HLSLType::DWord ||
		indexType.GetType() == HLSLType::Int))
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
		node.ResolveType(structureType.SubElement(node.GetRight()));
		break;
	default:
		throw HLSLParserException(node.GetToken(), "Not yet implemented");
		break;
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
		HLSLFunctionInputSignature inputSignature(types);
		HLSLType returnType = leftType.ReturnType(inputSignature);

		if (returnType.GetType() == HLSLType::Unknown)
		{
			throw HLSLParserException(node.GetToken(), "no overloaded function matches parameters");
		}
		else
		{
			node.ResolveType(returnType);
		}
	}
	else if (leftType.IsTypeClass())
	{
		if (leftType.GetType() == HLSLType::Vector)
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