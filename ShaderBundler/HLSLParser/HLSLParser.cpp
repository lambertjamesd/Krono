#include "HLSLParser.h"
#include "HLSLParserException.h"
#include "HLSLTypeVisitor.h"
#include <cassert>
#include <sstream>

HLSLParser::HLSLParser(std::istream& input) :
	mTokenizer(input),
	mCurrentToken(0)
{

}


HLSLParser::~HLSLParser(void)
{
}

void HLSLParser::Test()
{
	TestString(gParseTest);
}

void HLSLParser::TestString(const std::string& string)
{
	std::istringstream stream(string);
	HLSLParser tester(stream);
	std::unique_ptr<HLSLNode> file = move(tester.ParseFile());
}

const char* HLSLParser::gParseTest = "struct Foo { float4 bar; };";

std::unique_ptr<HLSLNode> HLSLParser::ParseFile()
{
	std::unique_ptr<HLSLNodeList> result(new HLSLNodeList());

	while (Peek(0).GetType() != HLSLTokenType::EndOfFile)
	{
		result->AddNode(move(ParseFileDeclaration()));
	}

	HLSLTypeVisitor typeGenerator;
	result->Accept(typeGenerator);

	return move(result);
}

size_t HLSLParser::GetParseLocation() const
{
	return mCurrentToken;
}

void HLSLParser::SetParseLocation(size_t value)
{
	mCurrentToken = value;
}

const HLSLToken& HLSLParser::Peek(size_t offset)
{
	return mTokenizer.GetToken(mCurrentToken + offset);
}

const HLSLToken& HLSLParser::Require(HLSLTokenType::Type type)
{
	const HLSLToken& result = Peek(0);

	if (result.GetType() == type)
	{
		Advance();
		return result;
	}
	else
	{
		throw HLSLParserException(result);
	}
}

const HLSLToken& HLSLParser::Require(HLSLKeyword::Type type)
{
	const HLSLToken& result = Peek(0);

	if (result.GetKeywordType() == type)
	{
		Advance();
		return result;
	}
	else
	{
		throw HLSLParserException(result);
	}
}

bool HLSLParser::Optional(HLSLTokenType::Type type)
{
	if (Peek(0).GetType() == type)
	{
		Advance(1);
		return true;
	}
	else
	{
		return false;
	}
}

bool HLSLParser::Optional(HLSLKeyword::Type type)
{
	if (Peek(0).GetKeywordType() == type)
	{
		Advance(1);
		return true;
	}
	else
	{
		return false;
	}
}

void HLSLParser::Advance(size_t count)
{
	mCurrentToken += count;
}

const HLSLToken& HLSLParser::Advance()
{
	const HLSLToken& result = Peek(0);
	++mCurrentToken;
	return result;
}

bool HLSLParser::DryParseType()
{
	const HLSLToken& nextToken = Peek(0);

	if (HLSLKeyword::IsTextureType(nextToken.GetKeywordType()) || 
		nextToken.GetKeywordType() == HLSLKeyword::SamplerState ||
		nextToken.GetKeywordType() == HLSLKeyword::Void)
	{
		Advance();
		return true;
	}
	else if (nextToken.GetKeywordType() == HLSLKeyword::Buffer)
	{
		Advance();
		return Optional(HLSLTokenType::LessThan) &&
			DryParseBasicType() &&
			Optional(HLSLTokenType::GreaterThan);
	}
	else
	{
		return DryParseBasicType();
	}
}

bool HLSLParser::DryParseBasicType()
{
	const HLSLToken& nextToken = Peek(0);

	if (nextToken.GetKeywordType() == HLSLKeyword::Vector)
	{
		Advance();
		return Optional(HLSLTokenType::LessThan) &&
			DryParseScalarType() &&
			Optional(HLSLTokenType::Comma) &&
			Optional(HLSLTokenType::Number) &&
			Optional(HLSLTokenType::GreaterThan);
	}
	else if (nextToken.GetKeywordType() == HLSLKeyword::Matrix)
	{
		Advance();
		return Optional(HLSLTokenType::LessThan) &&
			DryParseScalarType() &&
			Optional(HLSLTokenType::Comma) &&
			Optional(HLSLTokenType::Number) &&
			Optional(HLSLTokenType::Comma) &&
			Optional(HLSLTokenType::Number) &&
			Optional(HLSLTokenType::GreaterThan);
	}
	else
	{
		return DryParseScalarType();
	}
}

bool HLSLParser::DryParseScalarType()
{
	const HLSLToken& next = Peek(0);

	if (next.GetType() == HLSLTokenType::Identifier ||
		HLSLKeyword::IsScalarType(next.GetKeywordType()))
	{
		Advance();
		return true;
	}

	return false;
}

bool HLSLParser::MatchesType()
{
	size_t startPosition = GetParseLocation();
	bool result = DryParseType();
	SetParseLocation(startPosition);

	return result;

}

bool HLSLParser::MatchesVariableDeclaration()
{
	size_t startPosition = GetParseLocation();

	bool result = 
		DryParseType() &&
		Optional(HLSLTokenType::Identifier) && (
			Optional(HLSLTokenType::OpenSquare) ||
			Optional(HLSLTokenType::Colon) || 
			Optional(HLSLTokenType::SemiColon) || 
			Optional(HLSLTokenType::Assign) ||
			Optional(HLSLTokenType::LessThan));
	
	SetParseLocation(startPosition);

	return result;
}

bool HLSLParser::MatchesFunctionDeclaration()
{
	size_t startPosition = GetParseLocation();

	bool result = 
		DryParseType() &&
		Optional(HLSLTokenType::Identifier) &&
		Optional(HLSLTokenType::OpenParen);

	SetParseLocation(startPosition);

	return result;
}

bool HLSLParser::MatchesControlStatement()
{
	const HLSLToken& token = Peek(0);

	return token.GetKeywordType() >= HLSLKeyword::Break &&
		token.GetKeywordType() <= HLSLKeyword::While;
}

std::unique_ptr<HLSLNode> HLSLParser::ParseFileDeclaration()
{
	const HLSLToken& nextToken = Peek(0);

	switch (nextToken.GetKeywordType())
	{
	case HLSLKeyword::Struct:
		return move(ParseStruct());
	case HLSLKeyword::Cbuffer:
	case HLSLKeyword::Tbuffer:
		return move(ParseCBuffer());
	case HLSLKeyword::Typedef:
		return move(ParseTypedef());
	}
	
	if (MatchesVariableDeclaration())
	{
		std::unique_ptr<HLSLNode> result(move(ParseVariable()));
		Require(HLSLTokenType::SemiColon);
		return move(result);
	}
	else if (MatchesFunctionDeclaration())
	{
		return move(ParseFunction());
	}
	else
	{
		throw HLSLParserException(nextToken);
	}
}

std::unique_ptr<HLSLStructDefinition> HLSLParser::ParseStruct()
{
	const HLSLToken& startToken = Require(HLSLKeyword::Struct);
	std::string name = Require(HLSLTokenType::Identifier).GetValue();

	std::unique_ptr<HLSLStructDefinition> result(new HLSLStructDefinition(startToken, name));

	Require(HLSLTokenType::OpenCurly);

	while (!Optional(HLSLTokenType::CloseCurly))
	{
		if (Peek(0).GetType() == HLSLTokenType::EndOfFile)
		{
			throw HLSLParserException(Peek(0));
		}

		result->AddMember(move(ParseStructMember()));
		Require(HLSLTokenType::SemiColon);
	}

	Require(HLSLTokenType::SemiColon);

	return move(result);
}

std::unique_ptr<HLSLStructureMember> HLSLParser::ParseStructMember()
{
	const HLSLToken& startToken = Peek(0);
	InterpolationMode::Type interpolationMode = ParseInterpolationMode();
	std::unique_ptr<HLSLTypeNode> type = move(ParseType());
	const std::string& name = Require(HLSLTokenType::Identifier).GetValue();

	CheckForArrayType(type);

	std::unique_ptr<HLSLStructureMember> result(new HLSLStructureMember(startToken, interpolationMode, move(type), name));
	
	if (Optional(HLSLTokenType::Colon))
	{
		result->SetSemantic(Require(HLSLTokenType::Identifier).GetValue());
	}

	return move(result);
}

InterpolationMode::Type HLSLParser::ParseInterpolationMode()
{
	InterpolationMode::Type result = InterpolationMode::None;

	HLSLToken token = Peek(0);

	while (
		token.GetKeywordType() == HLSLKeyword::Linear ||
		token.GetKeywordType() == HLSLKeyword::Nointerpolation ||
		token.GetKeywordType() == HLSLKeyword::Noperspective ||
		token.GetKeywordType() == HLSLKeyword::Centroid ||
		token.GetKeywordType() == HLSLKeyword::Sample)
	{
		Advance();

		if (token.GetKeywordType() == HLSLKeyword::Linear)
		{
			if (result & InterpolationMode::InterpolationMask)
			{
				throw HLSLParserException(token);
			}
			result = InterpolationMode::Linear;
		}
		else if (token.GetKeywordType() == HLSLKeyword::Nointerpolation)
		{
			if (result & InterpolationMode::InterpolationMask)
			{
				throw HLSLParserException(token);
			}
			result = InterpolationMode::NoInterpolation;
		}
		else if (token.GetKeywordType() == HLSLKeyword::Noperspective)
		{
			if (result & InterpolationMode::InterpolationMask)
			{
				throw HLSLParserException(token);
			}
			result = InterpolationMode::NoPerspective;
		}
		else if (token.GetKeywordType() == HLSLKeyword::Centroid)
		{
			if (result & InterpolationMode::Centroid)
			{
				throw HLSLParserException(token);
			}
			result = (InterpolationMode::Type)(result | InterpolationMode::Centroid);
		}
		else if (token.GetKeywordType() == HLSLKeyword::Sample)
		{
			if (result & InterpolationMode::Sample)
			{
				throw HLSLParserException(token);
			}
			result = (InterpolationMode::Type)(result | InterpolationMode::Sample);
		}
		

		token = Peek(0);
	}

	return result;
}

std::unique_ptr<HLSLCBufferDefinition> HLSLParser::ParseCBuffer()
{
	const HLSLToken& startToken = Peek(0);

	if (!Optional(HLSLKeyword::Tbuffer))
	{
		Require(HLSLKeyword::Cbuffer);
	}

	std::string name;

	if (Peek(0).GetType() == HLSLTokenType::Identifier)
	{
		name = Require(HLSLTokenType::Identifier).GetValue();
	}

	std::unique_ptr<HLSLCBufferDefinition> result(new HLSLCBufferDefinition(startToken, name));

	if (Optional(HLSLTokenType::Colon))
	{
		result->SetRegister(ParseRegister());
	}

	Require(HLSLTokenType::OpenCurly);

	while (!Optional(HLSLTokenType::CloseCurly))
	{
		if (Peek(0).GetType() == HLSLTokenType::EndOfFile)
		{
			throw HLSLParserException(Peek(0));
		}

		result->AddMember(move(ParseVariable()));
		Require(HLSLTokenType::SemiColon);
	}

	Require(HLSLTokenType::SemiColon);

	return move(result);
}

HLSLRegisterLocation HLSLParser::ParseRegister()
{
	Require(HLSLKeyword::Register);
	Require(HLSLTokenType::OpenParen);

	const std::string registerName = Require(HLSLTokenType::Identifier).GetValue();
	size_t index = 0;

	if (Optional(HLSLTokenType::OpenSquare))
	{
		index = atoi(Require(HLSLTokenType::Number).GetValue().c_str());
		Require(HLSLTokenType::CloseSquare);
	}
	
	Require(HLSLTokenType::CloseParen);

	return HLSLRegisterLocation(registerName.substr(0, 1), atoi(registerName.c_str() + 1), index); 
}

HLSLRegisterLocation HLSLParser::ParsePackOffset()
{
	Require(HLSLKeyword::Packoffset);
	Require(HLSLTokenType::OpenParen);

	const HLSLToken& registerNameToken = Require(HLSLTokenType::Identifier);
	std::string registerName = registerNameToken.GetValue();
	size_t index = 0;

	if (registerName[0] != 'c')
	{
		throw HLSLParserException(registerNameToken);
	}

	if (Optional(HLSLTokenType::StructureOperator))
	{
		std::string offset = Require(HLSLTokenType::Identifier).GetValue();

		switch (offset[0])
		{
		case 'x': index = 0; break;
		case 'y': index = 1; break;
		case 'z': index = 2; break;
		case 'w': index = 3; break;
		}
	}

	return HLSLRegisterLocation(registerName.substr(0, 1), atoi(registerName.c_str() + 1), index); 
}

void HLSLParser::CheckForArrayType(std::unique_ptr<HLSLTypeNode>& type)
{
	if (Optional(HLSLTokenType::OpenSquare))
	{
		const HLSLToken& size = Require(HLSLTokenType::Number);
		std::unique_ptr<HLSLTypeNode> arrayType(new HLSLArrayTypeNode(size, move(type), size.GetIntValue()));
		Require(HLSLTokenType::CloseSquare);

		type = move(arrayType);
	}
}

std::unique_ptr<HLSLTypedefDefinition> HLSLParser::ParseTypedef()
{
	const HLSLToken& startToken = Require(HLSLKeyword::Typedef);
	std::unique_ptr<HLSLTypeNode> type(move(ParseType()));
	std::string name = Require(HLSLTokenType::Identifier).GetValue();

	CheckForArrayType(type);

	Require(HLSLTokenType::SemiColon);

	return std::unique_ptr<HLSLTypedefDefinition>(new HLSLTypedefDefinition(startToken, move(type), name));
}

std::unique_ptr<HLSLVariableDefinition> HLSLParser::ParseVariable()
{
	const HLSLToken& startToken = Peek(0);
	VariableStorageClass::Type storageClass = ParseVariableStorageClass();
	VariableTypeModifer::Type typeModifier = ParseVariableTypeModifier(); 
	std::unique_ptr<HLSLTypeNode> type = move(ParseType());
	const std::string& name = Require(HLSLTokenType::Identifier).GetValue();
	
	CheckForArrayType(type);

	std::unique_ptr<HLSLVariableDefinition> result(new HLSLVariableDefinition(startToken, storageClass, typeModifier, move(type), name));

	while (Peek(0).GetType() == HLSLTokenType::Colon)
	{
		if (Peek(1).GetType() == HLSLTokenType::Identifier)
		{
			Advance();
			result->SetSemantic(Require(HLSLTokenType::Identifier).GetValue());
		}
		else if (Peek(1).GetKeywordType() == HLSLKeyword::Register)
		{
			Advance();
			result->SetRegisterLocation(ParseRegister());
		}
	}

	if (Optional(HLSLTokenType::Assign))
	{
		result->SetInitialValue(move(ParseExpression()));
	}

	return move(result);
}

VariableStorageClass::Type HLSLParser::ParseVariableStorageClass()
{
	VariableStorageClass::Type result = VariableStorageClass::None;
	VariableStorageClass::Type currentValue = VariableStorageClass::None;

	do
	{
		if (currentValue != VariableStorageClass::None)
		{
			Advance();
		}

		HLSLToken nextToken = Peek(0);

		switch (nextToken.GetKeywordType())
		{
		case HLSLKeyword::Extern: currentValue = VariableStorageClass::Extern; break;
		case HLSLKeyword::Nointerpolation: currentValue = VariableStorageClass::NoInterpolation; break;
		case HLSLKeyword::Precise: currentValue = VariableStorageClass::Precise; break;
		case HLSLKeyword::Shared: currentValue = VariableStorageClass::Shared; break;
		case HLSLKeyword::Groupshared: currentValue = VariableStorageClass::GroupShared; break;
		case HLSLKeyword::Static: currentValue = VariableStorageClass::Static; break;
		case HLSLKeyword::Uniform: currentValue = VariableStorageClass::Uniform; break;
		case HLSLKeyword::Volatile: currentValue = VariableStorageClass::Volatile; break;

		default: currentValue = VariableStorageClass::None;
		}

		if (result & currentValue)
		{
			throw HLSLParserException(nextToken);
		}

		result = VariableStorageClass::Type(result | currentValue);

	} while (currentValue != VariableStorageClass::None);

	return result;
}

VariableTypeModifer::Type HLSLParser::ParseVariableTypeModifier()
{
	VariableTypeModifer::Type result = VariableTypeModifer::None;
	VariableTypeModifer::Type currentValue = VariableTypeModifer::None;

	do
	{
		if (currentValue != VariableTypeModifer::None)
		{
			Advance();
		}

		HLSLToken nextToken = Peek(0);

		switch (nextToken.GetKeywordType())
		{
		case HLSLKeyword::Const: currentValue = VariableTypeModifer::Const; break;
		case HLSLKeyword::Row_major: currentValue = VariableTypeModifer::RowMajor; break;
		case HLSLKeyword::Column_major: currentValue = VariableTypeModifer::ColumnMajor; break;

		default: currentValue = VariableTypeModifer::None;
		}

		if (result & currentValue)
		{
			throw HLSLParserException(nextToken);
		}

		result = VariableTypeModifer::Type(result | currentValue);

	} while (currentValue != VariableTypeModifer::None);

	return result;
}

HLSLFunctionParameter::InputModifier HLSLParser::ParseInputModifier()
{
	HLSLFunctionParameter::InputModifier result = HLSLFunctionParameter::None;

	const HLSLToken& next = Peek(0);

	switch (next.GetKeywordType())
	{
	case HLSLKeyword::In: result = HLSLFunctionParameter::In; Advance(); break;
	case HLSLKeyword::Out: result = HLSLFunctionParameter::Out; Advance(); break;
	case HLSLKeyword::Inout: result = HLSLFunctionParameter::InOut; Advance(); break;
	case HLSLKeyword::Uniform: result = HLSLFunctionParameter::Uniform; Advance(); break;
	}

	return result;
}

std::unique_ptr<HLSLFunctionParameter> HLSLParser::ParseFunctionParameter()
{
	const HLSLToken& startToken = Peek(0);
	HLSLFunctionParameter::InputModifier inputModifier = ParseInputModifier();
	std::unique_ptr<HLSLTypeNode> type(move(ParseType()));
	std::string name = Require(HLSLTokenType::Identifier).GetValue();

	CheckForArrayType(type);
	
	std::unique_ptr<HLSLFunctionParameter> result(new HLSLFunctionParameter(startToken, inputModifier, move(type), name));
	
	if (Optional(HLSLTokenType::Colon))
	{
		result->SetSemantic(Require(HLSLTokenType::Identifier).GetValue());
	}

	result->SetInterpolationMode(ParseInterpolationMode());

	if (Optional(HLSLTokenType::Assign))
	{
		result->SetInitialzier(move(ParseExpression()));
	}

	return result;
}

std::unique_ptr<HLSLFunctionDefinition> HLSLParser::ParseFunction()
{
	const HLSLToken& startToken = Peek(0);
	std::unique_ptr<HLSLTypeNode> returnType(move(ParseType()));
	std::string name = Require(HLSLTokenType::Identifier).GetValue();

	std::unique_ptr<HLSLFunctionDefinition> result(new HLSLFunctionDefinition(startToken, move(returnType), name));

	Require(HLSLTokenType::OpenParen);

	bool looping = !Optional(HLSLTokenType::CloseParen);
	bool optionalRequired = false;

	while (looping)
	{
		std::unique_ptr<HLSLFunctionParameter> parameter(move(ParseFunctionParameter()));

		if (optionalRequired && !parameter->IsOptional())
		{
			throw HLSLParserException(parameter->GetToken(), "parameter must be optional");
		}

		optionalRequired = optionalRequired || parameter->IsOptional();
		result->AddParameter(move(parameter));

		if (!Optional(HLSLTokenType::Comma))
		{
			Require(HLSLTokenType::CloseParen);
			looping = false;
		}
	}

	if (Optional(HLSLTokenType::Colon))
	{
		result->SetSemantic(Require(HLSLTokenType::Identifier).GetValue());
	}

	if (!Optional(HLSLTokenType::SemiColon))
	{
		result->SetBody(move(ParseBlock(false)));
	}

	return move(result);
}

std::unique_ptr<HLSLTypeNode> HLSLParser::ParseType()
{
	const HLSLToken& nextToken = Peek(0);

	if (HLSLKeyword::IsTextureType(nextToken.GetKeywordType()))
	{
		return std::unique_ptr<HLSLTypeNode>(new HLSLTextureTypeNode(Advance()));
	}
	else if (nextToken.GetKeywordType() == HLSLKeyword::SamplerState)
	{
		return std::unique_ptr<HLSLTypeNode>(new HLSLSamplerTypeNode(Advance()));
	}
	else if (nextToken.GetKeywordType() == HLSLKeyword::Buffer)
	{
		Advance();
		Require(HLSLTokenType::LessThan);
		std::unique_ptr<HLSLTypeNode> result(move(ParseBasicType()));
		Require(HLSLTokenType::GreaterThan);
		return move(result);
	}
	else if (nextToken.GetKeywordType() == HLSLKeyword::Void)
	{
		return std::unique_ptr<HLSLTypeNode>(new HLSLVoidNode(Advance()));
	}
	else
	{
		return move(ParseBasicType());
	}
}

std::unique_ptr<HLSLTypeNode> HLSLParser::ParseBasicType()
{
	const HLSLToken& nextToken = Peek(0);

	if (nextToken.GetKeywordType() == HLSLKeyword::Vector)
	{
		Advance();
		Require(HLSLTokenType::LessThan);
		std::unique_ptr<HLSLTypeNode> internalType(move(ParseScalarType()));
		Require(HLSLTokenType::Comma);
		size_t size = Require(HLSLTokenType::Number).GetIntValue();
		Require(HLSLTokenType::GreaterThan);

		return std::unique_ptr<HLSLTypeNode>(new HLSLVectorTypeNode(nextToken, move(internalType), size));
	}
	else if (nextToken.GetKeywordType() == HLSLKeyword::Matrix)
	{
		Advance();
		Require(HLSLTokenType::LessThan);
		std::unique_ptr<HLSLTypeNode> internalType(move(ParseScalarType()));
		Require(HLSLTokenType::Comma);
		size_t rows = Require(HLSLTokenType::Number).GetIntValue();
		Require(HLSLTokenType::Comma);
		size_t columns = Require(HLSLTokenType::Number).GetIntValue();
		Require(HLSLTokenType::GreaterThan);
		
		return std::unique_ptr<HLSLTypeNode>(new HLSLMatrixTypeNode(nextToken, move(internalType), rows, columns));
	}
	else
	{
		return move(ParseScalarType());
	}
}

std::unique_ptr<HLSLTypeNode> HLSLParser::ParseScalarType()
{
	const HLSLToken& next = Peek(0);

	if (next.GetType() == HLSLTokenType::Identifier)
	{
		return std::unique_ptr<HLSLTypeNode>(new HLSLNamedTypeNode(Advance()));
	}
	else if (HLSLKeyword::IsScalarType(next.GetKeywordType()))
	{
		return std::unique_ptr<HLSLTypeNode>(new HLSLScalarTypeNode(Advance()));
	}

	throw HLSLParserException(next);
}


std::unique_ptr<HLSLStatementBlock> HLSLParser::ParseBlock(bool allowSingleLine)
{
	bool isSingleLine = false;

	if (allowSingleLine)
	{
		isSingleLine = !Optional(HLSLTokenType::OpenCurly);
	}
	else
	{
		Require(HLSLTokenType::OpenCurly);
	}

	std::unique_ptr<HLSLStatementBlock> result(new HLSLStatementBlock());

	if (isSingleLine)
	{
		result->AddNode(move(ParseStatement()));
	}
	else
	{
		while (!Optional(HLSLTokenType::CloseCurly))
		{
			result->AddNode(move(ParseStatement()));
		}
	}

	return move(result);
}

std::unique_ptr<HLSLNode> HLSLParser::ParseStatement()
{
	const HLSLToken& nextToken = Peek(0);

	switch (nextToken.GetKeywordType())
	{
	case HLSLKeyword::Struct:
		return move(ParseStruct());
	case HLSLKeyword::Cbuffer:
	case HLSLKeyword::Tbuffer:
		throw HLSLParserException(nextToken, "cbuffer not allowed in function");
	case HLSLKeyword::Typedef:
		return move(ParseTypedef());
	case HLSLKeyword::Return:
		{
			Advance();
			std::unique_ptr<HLSLNode> result(new HLSLReturnStatement(nextToken, move(ParseExpression())));
			Require(HLSLTokenType::SemiColon);
			return move(result);
		}
	}
	
	if (MatchesVariableDeclaration())
	{
		std::unique_ptr<HLSLNode> result(move(ParseVariable()));
		Require(HLSLTokenType::SemiColon);
		return move(result);
	}
	else if (MatchesControlStatement())
	{
		return move(ParseFlowControl());
	}
	else if (MatchesFunctionDeclaration())
	{
		throw HLSLParserException(nextToken, "cannot have nested function");
	}
	else
	{
		std::unique_ptr<HLSLNode> result(move(ParseExpression()));
		Require(HLSLTokenType::SemiColon);
		return move(result);
	}
}

std::unique_ptr<HLSLFlowControlNode> HLSLParser::ParseFlowControl()
{
	const HLSLToken& token = Peek(0);

	switch (token.GetKeywordType())
	{
	case HLSLKeyword::Do:
		return move(ParseDoBlock());
	case HLSLKeyword::If:
		return move(ParseIfBlock());
	case HLSLKeyword::For:
		return move(ParseForBlock());
	case HLSLKeyword::Switch:
		return move(ParseSwitchBlock());
	case HLSLKeyword::While:
		return move(ParseWhileBlock());
	case HLSLKeyword::Case:
		return move(ParseSwitchLabel());
	case HLSLKeyword::Default:
		{
			std::unique_ptr<HLSLDefaultNode> result(new HLSLDefaultNode(Advance()));
			Require(HLSLTokenType::Colon);
			return move(result);
		}
	case HLSLKeyword::Break:
		{
			std::unique_ptr<HLSLBreakNode> result(new HLSLBreakNode(Advance()));
			Require(HLSLTokenType::SemiColon);
			return move(result);
		}
	case HLSLKeyword::Continue:
		{
			std::unique_ptr<HLSLContinueNode> result(new HLSLContinueNode(Advance()));
			Require(HLSLTokenType::SemiColon);
			return move(result);
		}
	case HLSLKeyword::Discard:
		{
			std::unique_ptr<HLSLDiscardNode> result(new HLSLDiscardNode(Advance()));
			Require(HLSLTokenType::SemiColon);
			return move(result);
		}
	}

	throw HLSLParserException(token);
}

std::unique_ptr<HLSLDoNode> HLSLParser::ParseDoBlock()
{
	const HLSLToken& startToken = Require(HLSLKeyword::Do);
	std::unique_ptr<HLSLNode> body(move(ParseBlock(true)));
	Require(HLSLKeyword::While);
	Require(HLSLTokenType::OpenParen);
	std::unique_ptr<HLSLExpressionNode> condition(move(ParseExpression()));
	Require(HLSLTokenType::CloseParen);
	Require(HLSLTokenType::SemiColon);

	return std::unique_ptr<HLSLDoNode>(new HLSLDoNode(startToken, move(body), move(condition)));
}

std::unique_ptr<HLSLIfNode> HLSLParser::ParseIfBlock()
{
	const HLSLToken& startToken = Require(HLSLKeyword::If);

	Require(HLSLTokenType::OpenParen);
	std::unique_ptr<HLSLExpressionNode> condition(move(ParseExpression()));
	Require(HLSLTokenType::CloseParen);

	std::unique_ptr<HLSLNode> body(move(ParseBlock(true)));

	std::unique_ptr<HLSLIfNode> result(new HLSLIfNode(startToken, move(condition), move(body)));

	if (Optional(HLSLKeyword::Else))
	{
		result->SetElseBody(move(ParseBlock(true)));
	}

	return move(result);
}

std::unique_ptr<HLSLForNode> HLSLParser::ParseForBlock()
{
	const HLSLToken& startToken = Require(HLSLKeyword::For);
	
	Require(HLSLTokenType::OpenParen);
	std::unique_ptr<HLSLStatementNode> initializer;

	if (MatchesVariableDeclaration())
	{
		initializer = move(ParseVariable());
	}
	else
	{
		initializer = move(ParseExpression());
	}

	Require(HLSLTokenType::SemiColon);
	std::unique_ptr<HLSLExpressionNode> condition(move(ParseExpression()));
	Require(HLSLTokenType::SemiColon);
	std::unique_ptr<HLSLExpressionNode> iterator(move(ParseExpression()));
	Require(HLSLTokenType::CloseParen);
	
	std::unique_ptr<HLSLNode> body(move(ParseBlock(true)));

	return std::unique_ptr<HLSLForNode>(new HLSLForNode(startToken,
		move(initializer),
		move(condition),
		move(iterator),
		move(body)));
}

std::unique_ptr<HLSLSwitchNode> HLSLParser::ParseSwitchBlock()
{
	const HLSLToken& startToken = Require(HLSLKeyword::Switch);

	Require(HLSLTokenType::OpenParen);
	std::unique_ptr<HLSLExpressionNode> selectValue(move(ParseExpression()));
	Require(HLSLTokenType::CloseParen);

	std::unique_ptr<HLSLNode> body(move(ParseBlock(true)));

	return std::unique_ptr<HLSLSwitchNode>(new HLSLSwitchNode(startToken, move(selectValue), move(body)));
}

std::unique_ptr<HLSLWhileNode> HLSLParser::ParseWhileBlock()
{
	const HLSLToken& startToken = Require(HLSLKeyword::While);

	Require(HLSLTokenType::OpenParen);
	std::unique_ptr<HLSLExpressionNode> condition(move(ParseExpression()));
	Require(HLSLTokenType::CloseParen);

	std::unique_ptr<HLSLNode> body(move(ParseBlock(true)));

	return std::unique_ptr<HLSLWhileNode>(new HLSLWhileNode(startToken, move(condition), move(body)));
}

std::unique_ptr<HLSLSwitchLabelNode> HLSLParser::ParseSwitchLabel()
{
	const HLSLToken& startToken = Require(HLSLKeyword::Case);

	int value;
	
	if (Optional(HLSLTokenType::Minus))
	{
		value = -Require(HLSLTokenType::Number).GetIntValue();
	}
	else
	{
		value = Require(HLSLTokenType::Number).GetIntValue();
	}
	
	Require(HLSLTokenType::Colon);
	return std::unique_ptr<HLSLSwitchLabelNode>(new HLSLSwitchLabelNode(startToken, value));
}

bool HLSLParser::MatchesCast()
{
	size_t startPos = GetParseLocation();

	bool result = Optional(HLSLTokenType::OpenParen) && 
		DryParseType() && 
		Optional(HLSLTokenType::CloseParen) &&
		!Peek(0).IsBinaryOperator();

	SetParseLocation(startPos);

	return result;
}

std::unique_ptr<HLSLExpressionNode> HLSLParser::ParseExpression()
{
	return move(ParseBinaryOperator(OperatorPrecedence::Comma));
}

std::unique_ptr<HLSLExpressionNode> HLSLParser::ParseBinaryOperator(OperatorPrecedence::Type minPrecedence)
{
	std::unique_ptr<HLSLExpressionNode> result(move(ParseUnaryOperator()));

	
	while (Peek(0).IsBinaryOperator())
	{
		const HLSLToken& operatorToken = Peek(0);
		OperatorPrecedence::Type precedence = operatorToken.GetOperatorPrecedence();

		if (precedence >= minPrecedence)
		{
			const HLSLToken& binaryOperator = Advance();
			std::unique_ptr<HLSLExpressionNode> operandA(move(result));
			std::unique_ptr<HLSLExpressionNode> operandB(move(ParseBinaryOperator((OperatorPrecedence::Type)(minPrecedence + 1))));
	
			result = move(HLSLBinaryOperatorNode::CreateNode(operatorToken, move(operandA), move(operandB)));
		}
		else
		{
			break;
		}
	}

	return move(result);
}

std::unique_ptr<HLSLExpressionNode> HLSLParser::ParseUnaryOperator()
{
	if (Peek(0).IsUnaryOperator())
	{
		const HLSLToken& operatorToken = Advance();
		return move(HLSLUnaryOperator::CreateNode(operatorToken, move(ParseUnaryOperator())));
	}
	else if (MatchesCast())
	{
		const HLSLToken& startToken = Require(HLSLTokenType::OpenParen);
		std::unique_ptr<HLSLTypeNode> type(move(ParseType()));
		Require(HLSLTokenType::CloseParen);

		return std::unique_ptr<HLSLExpressionNode>(new HLSLCastNode(startToken, move(type), move(ParseUnaryOperator())));
	}
	else
	{
		return move(ParsePrefixExpression());
	}
}

std::unique_ptr<HLSLExpressionNode> HLSLParser::ParsePrefixExpression()
{
	if (Peek(0).IsPrefixOperator())
	{
		const HLSLToken& token = Advance();

		switch (token.GetType())
		{
		case HLSLTokenType::PlusPlus:
			return std::unique_ptr<HLSLExpressionNode>(new HLSLPrefixIncrNode(token, ParsePrefixExpression()));
		case HLSLTokenType::MinusMinus:
			return std::unique_ptr<HLSLExpressionNode>(new HLSLPrefixDecrNode(token, ParsePrefixExpression()));
		default:
			throw HLSLParserException(token, "invalid prefix operator");
		}
	}
	else
	{
		return move(ParsePostfixExpression());
	}
}

std::unique_ptr<HLSLExpressionNode> HLSLParser::ParsePostfixExpression()
{
	std::unique_ptr<HLSLExpressionNode> result(move(ParseValueExpression()));

	while (Peek(0).IsPostfixOperator())
	{
		const HLSLToken& token = Advance();
		std::unique_ptr<HLSLExpressionNode> left(move(result));

		switch (token.GetType())
		{
		case HLSLTokenType::PlusPlus:
			result.reset(new HLSLPostfixIncr(token, move(left)));
			break;
		case HLSLTokenType::MinusMinus:
			result.reset(new HLSLPostfixDecr(token, move(left)));
			break;
		case HLSLTokenType::OpenSquare:
			result.reset(new HLSLIndexNode(token, move(left), move(ParseExpression())));
			Require(HLSLTokenType::CloseSquare);
			break;
		case HLSLTokenType::StructureOperator:
			result.reset(new HLSLStructureNode(token, move(left), Require(HLSLTokenType::Identifier).GetValue()));
			break;
		case HLSLTokenType::OpenParen:
			{
				std::unique_ptr<HLSLFunctionCallNode> function(new HLSLFunctionCallNode(token, move(left)));
				bool looping = !Optional(HLSLTokenType::CloseParen);

				while (looping)
				{
					function->AddParameter(move(ParseBinaryOperator(OperatorPrecedence::Assign)));

					if (!Optional(HLSLTokenType::Comma))
					{
						Require(HLSLTokenType::CloseParen);
						looping = false;
					}
				}

				result = move(function);
			}
			break;
		default:
			throw HLSLParserException(token, "invalid postifx operator");
		}
	}

	return move(result);
}

std::unique_ptr<HLSLExpressionNode> HLSLParser::ParseValueExpression()
{
	const HLSLToken& token = Peek(0);

	if (token.GetType() == HLSLTokenType::Identifier)
	{
		return std::unique_ptr<HLSLExpressionNode>(new HLSLIdentifierNode(Advance()));
	}
	else if (token.GetType() == HLSLTokenType::Number)
	{
		return std::unique_ptr<HLSLExpressionNode>(new HLSLNumberConstantNode(Advance()));
	}
	else if (token.GetKeywordType() == HLSLKeyword::True || token.GetKeywordType() == HLSLKeyword::False)
	{
		return std::unique_ptr<HLSLExpressionNode>(new HLSLBooleanConstantNode(Advance()));
	}
	else if (token.GetType() == HLSLTokenType::OpenParen)
	{
		Advance();
		std::unique_ptr<HLSLExpressionNode> result(new HLSLParenthesisNode(token, move(ParseExpression())));
		Require(HLSLTokenType::CloseParen);
		return move(result);
	}
	else if (MatchesType())
	{
		return std::unique_ptr<HLSLExpressionNode>(new HLSLTypeExpressionNode(token, move(ParseType())));
	}
	else
	{
		throw HLSLParserException(token, "unexpected token");
	}
}
