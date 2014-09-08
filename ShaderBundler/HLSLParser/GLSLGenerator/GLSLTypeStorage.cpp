#include "GLSLTypeStorage.h"
#include "HLSLParser/HLSLParserException.h"
#include "HLSLParser/HLSLTypeNode.h"

#include <algorithm>

using namespace std;

GLSLTypeStorage::GLSLTypeStorage(void)
{
	BeginScope();

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

GLSLTypeStorage::~GLSLTypeStorage(void)
{

}

void GLSLTypeStorage::BeginScope()
{
	mScopeDefines.push_back(vector<string>());
}

void GLSLTypeStorage::EndScope()
{
	vector<string>& scopeDefines = mScopeDefines.back();

	for (auto it = scopeDefines.begin(); it != scopeDefines.end(); ++it)
	{
		mDefinedValues[*it].pop_back();
	}

	mScopeDefines.pop_back();
}

void GLSLTypeStorage::Define(const string& name, const shared_ptr<HLSLNode>& value)
{
	vector<string>& scopeDefines = mScopeDefines.back();

	if (find(scopeDefines.begin(), scopeDefines.end(), name) == scopeDefines.end())
	{
		mDefinedValues[name].push_back(value);
		scopeDefines.push_back(name);
	}
	else
	{
		throw HLSLParserException(value->GetToken(), name + " is already defined");
	}
}

void GLSLTypeStorage::DefineFunction(HLSLFunctionDefinition& value)
{
	vector<string>& scopeDefines = mScopeDefines.back();

	if (find(scopeDefines.begin(), scopeDefines.end(), value.GetName()) == scopeDefines.end())
	{
		mDefinedValues[value.GetName()].push_back(std::shared_ptr<HLSLTypeNode>(new HLSLFunctionTypeNode(value)));
		scopeDefines.push_back(value.GetName());
	}
	else
	{
		throw HLSLParserException(value.GetToken(), value.GetName() + " is already defined");
	}
}

HLSLNode* GLSLTypeStorage::GetValue(const string& name)
{
	if (mDefinedValues[name].size() > 0)
	{
		return mDefinedValues[name].back().get();
	}
	else
	{
		return NULL;
	}
}

std::unique_ptr<HLSLScalarTypeNode> GLSLTypeStorage::CreateScalarType(HLSLKeyword::Type scalarType)
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

std::shared_ptr<HLSLNode> GLSLTypeStorage::CreateVectorType(HLSLKeyword::Type scalarType, size_t size)
{
	std::unique_ptr<HLSLScalarTypeNode> scalarNode(move(CreateScalarType(scalarType)));
	return std::shared_ptr<HLSLVectorTypeNode>(new HLSLVectorTypeNode(HLSLToken(HLSLTokenType::Identifier, "vector", 0), move(scalarNode), size));
}

std::shared_ptr<HLSLNode> GLSLTypeStorage::CreateMatrixType(HLSLKeyword::Type scalarType, size_t rows, size_t columns)
{
	std::unique_ptr<HLSLScalarTypeNode> scalarNode(move(CreateScalarType(scalarType)));
	return std::shared_ptr<HLSLMatrixTypeNode>(new HLSLMatrixTypeNode(HLSLToken(HLSLTokenType::Identifier, "vector", 0), move(scalarNode), rows, columns));
}