#include "Preprocessor.h"
#include <cstring>
#include <cassert>
#include <sstream>
#include <iostream>
#include <fstream>
#include "GenerateOutputVisitor.h"

using namespace std;

namespace preproc
{

Preprocessor::Preprocessor(std::istream& stream) :
	mTokens(stream),
	mCurrentToken(0),
	mRootNode(move(ParseFile()))
{

}

Preprocessor::~Preprocessor(void)
{

}

std::string Preprocessor::PreprocessFile(const std::string& filename)
{
	std::ifstream inputStream(filename, std::ios::binary);
	Preprocessor preprocessor(inputStream);
	
	IncludeHandler includeHandler("", filename);
	MacroStorage storage;
	return GenerateOutputVisitor::Process(preprocessor.GetRootNode(), storage, includeHandler);
}

std::string Preprocessor::PreprocessStream(std::istream& input, MacroStorage& storage, IncludeHandler& includeHandler)
{
	Preprocessor preprocessor(input);
	return GenerateOutputVisitor::Process(preprocessor.GetRootNode(), storage, includeHandler);
}

Node& Preprocessor::GetRootNode()
{
	return *mRootNode;
}

void Preprocessor::Test()
{
	Test("#if MONKEY\nfoo bar\n#elif FOO\nbar\n#endif");
}

void Preprocessor::Test(const char* input)
{
	try
	{
		std::istringstream inputStream(input);
		Preprocessor preprocessor(inputStream);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

size_t Preprocessor::GetLocation()
{
	return mCurrentToken;
}

void Preprocessor::RestoreLocation(size_t location)
{
	mCurrentToken = location;
}

const Token& Preprocessor::PeekNoWhite(size_t offset)
{
	size_t peekOffset = 0;

	while (Peek(peekOffset).GetType() == Token::Whitespace || offset > 0)
	{
		if (Peek(0).GetType() != Token::Whitespace)
		{
			--offset;
		}

		++peekOffset;
	}

	return Peek(peekOffset);
}

const Token& Preprocessor::RequireNoWhite(Token::Type type)
{
	while (Peek(0).GetType() == Token::Whitespace)
	{
		Advance((size_t)1);
	}

	return Require(type);
}

void Preprocessor::AdvanceNoWhite(size_t count)
{
	while (Peek(0).GetType() == Token::Whitespace || count > 0)
	{
		if (Peek(0).GetType() != Token::Whitespace)
		{
			--count;
		}

		Advance((size_t)1);
	}
}

const Token& Preprocessor::AdvanceNoWhite()
{
	while (Peek(0).GetType() == Token::Whitespace)
	{
		Advance((size_t)1);
	}
	
	return Advance();
}

bool Preprocessor::OptionalNoWhite(Token::Type type)
{
	if (PeekNoWhite(0).GetType() == type)
	{
		Advance((size_t)1);
		return true;
	}
	else
	{
		return false;
	}
}

const Token& Preprocessor::Peek(size_t offset)
{
	return mTokens.GetToken(mCurrentToken + offset);
}

const Token& Preprocessor::Require(Token::Type type)
{
	const Token& result = Peek(0);

	if (result.GetType() == type)
	{
		++mCurrentToken;
		return result;
	}
	else
	{
		throw Exception(result, "");
	}
}

void Preprocessor::Advance(size_t count)
{
	mCurrentToken += count;
}

const Token& Preprocessor::Advance()
{
	const Token& result = Peek(0);
	++mCurrentToken;
	return result;
}

bool Preprocessor::Optional(Token::Type type)
{
	const Token& result = Peek(0);

	if (result.GetType() == type)
	{
		++mCurrentToken;
		return true;
	}
	else
	{
		return false;
	}
}

const Token& Preprocessor::Peek(size_t offset, bool ignoreWhitespace)
{
	if (ignoreWhitespace)
	{
		return PeekNoWhite(offset);
	}
	else
	{
		return Peek(offset);
	}
}

const Token& Preprocessor::Require(Token::Type type, bool ignoreWhitespace)
{
	if (ignoreWhitespace)
	{
		return RequireNoWhite(type);
	}
	else
	{
		return Require(type);
	}
}

void Preprocessor::Advance(size_t count, bool ignoreWhitespace)
{
	if (ignoreWhitespace)
	{
		AdvanceNoWhite(count);
	}
	else
	{
		Advance(count);
	}
}

const Token& Preprocessor::Advance(bool ignoreWhitespace)
{
	if (ignoreWhitespace)
	{
		return AdvanceNoWhite();
	}
	else
	{
		return Advance();
	}
}

bool Preprocessor::Optional(Token::Type type, bool ignoreWhitespace)
{
	if (ignoreWhitespace)
	{
		return OptionalNoWhite(type);
	}
	else
	{
		return Optional(type);
	}
}

bool Preprocessor::IsEndDirective()
{
	const Token& token = Peek(0);
	return token.GetType() == Token::EndOfFile ||
		token.GetType() == Token::NewLine;
}

bool Preprocessor::CheckLineExtend()
{
	if (Peek(0).GetType() == Token::BackSlash &&
		Peek(1).GetType() == Token::NewLine)
	{
		Advance((size_t)2);
		return true;
	}
	else
	{
		return false;
	}
}

void Preprocessor::ConsumeTillEndline()
{
	while (Optional(Token::Whitespace));

	if (!Optional(Token::EndOfFile))
	{
		Require(Token::NewLine);
	}
}

std::unique_ptr<NodeList> Preprocessor::ParseFile()
{
	return move(ParseBlock([](const Token& token) {
		return token.GetType() != Token::EndOfFile;
	}, "Expected end of file"));
}

std::unique_ptr<NodeList> Preprocessor::ParseBlock(Preprocessor::TokenChecker shouldContinue, const char* eofMessage)
{
	std::unique_ptr<NodeList> result(new NodeList());
	
	Token token = Peek(0);

	while (shouldContinue(token))
	{
		if (token.GetType() == Token::EndOfFile)
		{
			throw Exception(token, eofMessage);
		}

		if (token.GetType() == Token::Directive)
		{
			result->AddNode(move(ParseDirective()));
		}
		else if (token.GetType() == Token::Identifier)
		{
			result->AddNode(move(ParseIdentifier(false)));
		}
		else
		{
			result->AddNode(std::unique_ptr<Node>(new OtherNode(token.GetValue())));
			Advance((size_t)1);
		}

		token = Peek(0);
	}

	return move(result);
}

std::unique_ptr<DirectiveNode> Preprocessor::ParseDirective()
{
	const Token& token = Require(Token::Directive);

	if (token.GetValue() == "#define")
	{
		return move(ParseDefine());
	}
	else if (token.GetValue() == "#error")
	{
		ParseError();
		return std::unique_ptr<DirectiveNode>();
	}
	else if (token.GetValue() == "#if")
	{
		return move(ParseIf());
	}
	else if (token.GetValue() == "#ifdef")
	{
		return move(ParseIfDef());
	}
	else if (token.GetValue() == "#ifndef")
	{
		return move(ParseIfNDef());
	}
	else if (token.GetValue() == "#include")
	{
		return move(ParseInclude());
	}
	else if (token.GetValue() == "#line")
	{
		return move(ParseLineDef());
	}
	else if (token.GetValue() == "#pragma")
	{
		return move(ParsePragma());
	}
	else if (token.GetValue() == "#undef")
	{
		return move(ParseUnDef());
	}
	else if (token.GetValue() == "#elif" || token.GetValue() == "#else")
	{
		throw Exception(token, " else without matching if");
	}
	else if (token.GetValue() == "#endif")
	{
		throw Exception(token, " endif without matching if");
	}
	else
	{
		throw Exception(token, " invalid Preprocessor directive");
	}
}

std::unique_ptr<DefineNode> Preprocessor::ParseDefine()
{
	Require(Token::Whitespace);
	const Token& name = Require(Token::Identifier);
	std::vector<std::string> parameters;
	std::unique_ptr<NodeList> contents;
	std::unique_ptr<ExpressionNode> expressionValue;

	bool hasParameters = false;

	if (Optional(Token::OpenParen))
	{
		hasParameters = true;

		Optional(Token::Whitespace);

		bool isLooping = Optional(Token::CloseParen);

		while (isLooping)
		{
			Optional(Token::Whitespace);
			parameters.push_back(Require(Token::Identifier).GetValue());
			Optional(Token::Whitespace);

			if (!Optional(Token::Comma))
			{
				Require(Token::CloseParen);
				isLooping = false;
			}
		}
	}

	Optional(Token::Whitespace);

	if (Peek(0).GetType() == Token::EndOfFile || Peek(0).GetType() == Token::NewLine)
	{
		expressionValue.reset(new ConstantNode("0"));
	}
	else
	{
		size_t valueLocation = GetLocation();

		try
		{
			expressionValue = move(ParseBinaryOperator());
		}
		catch (Exception&)
		{

		}

		RestoreLocation(valueLocation);
	}

	while (!IsEndDirective())
	{
		if (!CheckLineExtend())
		{
			contents->AddNode(move(ParseDefinitionValue()));
		}
	}

	return std::unique_ptr<DefineNode>(new DefineNode(name.GetValue(), parameters, hasParameters, move(contents), move(expressionValue)));
}

std::unique_ptr<IfNode> Preprocessor::ParseIf()
{
	Require(Token::Whitespace);

	std::unique_ptr<ExpressionNode> expression(move(ParseBinaryOperator()));
	ConsumeTillEndline();
	return move(ParseIfBody(move(expression)));
}

std::unique_ptr<IfNode> Preprocessor::ParseIfDef()
{
	Require(Token::Whitespace);
	const Token& token = Require(Token::Identifier);
	ConsumeTillEndline();
	return move(ParseIfBody(std::unique_ptr<ExpressionNode>(new DefinedOperatorNode(token.GetValue()))));
}

std::unique_ptr<IfNode> Preprocessor::ParseIfNDef()
{
	Require(Token::Whitespace);
	const Token& token = Require(Token::Identifier);
	ConsumeTillEndline();

	std::unique_ptr<ExpressionNode> defined(new DefinedOperatorNode(token.GetValue()));

	return move(ParseIfBody(std::unique_ptr<ExpressionNode>(new BooleanNotOperatorNode(move(defined)))));
}

std::unique_ptr<IfNode> Preprocessor::ParseIfBody(std::unique_ptr<ExpressionNode> expression)
{
	std::unique_ptr<Node> body = move(ParseBlock([](const Token& token) {
		return token.GetType() != Token::Directive || (
			token.GetValue() != "#else" &&
			token.GetValue() != "#elif" &&
			token.GetValue() != "#endif");
	}, "Unexpected end of file, expected #endif"));

	Token nextBlock = Require(Token::Directive);

	std::unique_ptr<Node> elseBlock;

	if (nextBlock.GetValue() == "#elif")
	{
		elseBlock = move(ParseIf());
	}
	else if (nextBlock.GetValue() == "#else")
	{
		ConsumeTillEndline();

		elseBlock = move(ParseBlock([](const Token& token) {
			return token.GetType() != Token::Directive ||
				token.GetValue() != "#endif";
		}, "Unexpected end of file, expected #endif"));

		if (Require(Token::Directive).GetValue() != "#endif")
		{
			assert(false && "Unexpected directive");
		}

		ConsumeTillEndline();
	}
	else if (nextBlock.GetValue() == "#endif")
	{
		ConsumeTillEndline();
	}
	else
	{
		assert(false && "Unexpected directive");
	}

	return std::unique_ptr<IfNode>(new IfNode(move(expression), move(body), move(elseBlock)));
}

void Preprocessor::ParseError()
{
	Optional(Token::Whitespace);
	std::string message;
	while (!IsEndDirective())
	{
		if (!CheckLineExtend())
		{
			message += Peek(0).GetValue();
			Advance((size_t)1);
		}
	}

	throw Exception(message.c_str());
}

std::unique_ptr<IncludeNode> Preprocessor::ParseInclude()
{
	Require(Token::Whitespace);
	const Token& next = Peek(0);

	if (next.GetType() == Token::String)
	{
		Advance((size_t)1);
		std::string includePath = next.GetValue();

		ConsumeTillEndline();

		return std::unique_ptr<IncludeNode>(new IncludeNode(
			includePath.substr(1, includePath.length() - 2),
			IncludeNode::LocalInclude));
	}
	else if (next.GetType() == Token::LessThan)
	{
		std::string includePath = next.GetValue();

		Advance((size_t)1);

		while (!Optional(Token::GreaterThan))
		{
			includePath += Peek(0).GetValue();
			Advance((size_t)1);
		}

		ConsumeTillEndline();
		
		return std::unique_ptr<IncludeNode>(new IncludeNode(
			includePath,
			IncludeNode::SystemInclude));
	}
	else
	{
		throw Exception(next, "Expected include file");
	}
}


std::unique_ptr<LineNode> Preprocessor::ParseLineDef()
{
	Require(Token::Whitespace);
	size_t line = atoi(Require(Token::Other).GetValue().c_str());
	std::string filename;

	if (Optional(Token::Whitespace) && Peek(0).GetType() == Token::String)
	{
		filename = Peek(0).GetValue();
	}

	ConsumeTillEndline();

	return std::unique_ptr<LineNode>(new LineNode(line, filename));
}

std::unique_ptr<PragmaNode> Preprocessor::ParsePragma()
{
	while (!IsEndDirective())
	{
		if (!CheckLineExtend())
		{
			Advance((size_t)1);
		}
	}

	return std::unique_ptr<PragmaNode>(new PragmaNode());
}

std::unique_ptr<UnDefNode> Preprocessor::ParseUnDef()
{
	Require(Token::Whitespace);

	std::unique_ptr<UnDefNode> result(new UnDefNode(Require(Token::Identifier).GetValue()));

	ConsumeTillEndline();

	return move(result);
}

std::unique_ptr<Node> Preprocessor::ParseDefinitionValue()
{
	if (Peek(0).GetType() == Token::Identifier)
	{
		return move(ParseIdentifier(false));
	}
	else
	{
		std::unique_ptr<Node> result(new OtherNode(Peek(0).GetValue()));
		Advance((size_t)1);
		return move(result);
	}
}

std::unique_ptr<ExpressionNode> Preprocessor::ParseValue()
{
	const Token& next = PeekNoWhite(0);

	switch(next.GetType())
	{
	case Token::Number:
		AdvanceNoWhite();
		return std::unique_ptr<ExpressionNode>(new ConstantNode(next.GetValue()));
	case Token::Identifier:
		return move(ParseIdentifier(true));
	case Token::OpenParen:
		return move(ParseParenthesis());
	default:
		throw Exception(next, " expecting number or identifier");
	}
}

std::unique_ptr<ExpressionNode> Preprocessor::ParseParenthesis()
{
	RequireNoWhite(Token::OpenParen);
	std::unique_ptr<ExpressionNode> result(move(ParseBinaryOperator()));
	RequireNoWhite(Token::CloseParen);
	return move(result);
}

std::unique_ptr<ExpressionNode> Preprocessor::ParseIdentifier(bool useExpression)
{
	if (Peek(1, useExpression).GetType() == Token::OpenParen)
	{
		return move(ParseFunctionCall(useExpression));
	}
	else
	{
		return std::unique_ptr<ExpressionNode>(new IdentifierNode(Require(Token::Identifier, useExpression).GetValue()));
	}
}

std::unique_ptr<Node> Preprocessor::ParseFunctionParameter(bool useExpression)
{
	if (useExpression)
	{
		return ParseBinaryOperator();
	}
	else
	{
		std::unique_ptr<NodeList> result(new NodeList());

		Token next = Peek(0);

		while (next.GetType() != Token::Comma && 
			next.GetType() != Token::CloseParen)
		{
			if (next.GetType() == Token::EndOfFile)
			{
				throw Exception(next, "unexpected end of file");
			}
			result->AddNode(move(ParseDefinitionValue()));
			next = Peek(0);
		}

		return move(result);
	}
}

std::unique_ptr<ExpressionNode> Preprocessor::ParseFunctionCall(bool useExpression)
{
	std::unique_ptr<FunctionNode> result(new FunctionNode(Require(Token::Identifier).GetValue()));
	
	Require(Token::OpenParen, useExpression);

	while (!Optional(Token::CloseParen, useExpression))
	{
		result->AddParameter(move(ParseFunctionParameter(useExpression)));
	}

	return move(result);
}

std::unique_ptr<ExpressionNode> Preprocessor::ParseUnaryOperator()
{
	if (PeekNoWhite(0).IsUnaryOperator())
	{
		const Token& unaryOperator = AdvanceNoWhite();

		switch (unaryOperator.GetType())
		{
		case Token::BitNot:
			return std::unique_ptr<UnaryOperatorNode>(new BitwiseNotOperatorNode(move(ParseUnaryOperator())));
		case Token::BooleanNot:
			return std::unique_ptr<UnaryOperatorNode>(new BooleanNotOperatorNode(move(ParseUnaryOperator())));
		case Token::Minus:
			return std::unique_ptr<UnaryOperatorNode>(new NegateOperatorNode(move(ParseUnaryOperator())));
		default:
			throw Exception(unaryOperator, " invalid unary operator");
		}
	}
	else if (PeekNoWhite(0).GetValue() == "defined")
	{
		AdvanceNoWhite();

		bool needParen = OptionalNoWhite(Token::OpenParen);
		std::string name = RequireNoWhite(Token::Identifier).GetValue();

		if (needParen)
		{
			RequireNoWhite(Token::CloseParen);
		}

		return std::unique_ptr<DefinedOperatorNode>(new DefinedOperatorNode(name));
	}
	else
	{
		return move(ParseValue());
	}
}

std::unique_ptr<ExpressionNode> Preprocessor::ParseBinaryOperator(Token::OperatorPrecedence minPrecedence)
{
	std::unique_ptr<ExpressionNode> result(move(ParseUnaryOperator()));

	while (PeekNoWhite(0).IsBinaryOperator())
	{
		Token::OperatorPrecedence precedence = PeekNoWhite(0).GetPrecedence();

		if (precedence >= minPrecedence)
		{
			const Token& binaryOperator = AdvanceNoWhite();
			std::unique_ptr<ExpressionNode> operandA(move(result));
			std::unique_ptr<ExpressionNode> operandB(move(ParseBinaryOperator((Token::OperatorPrecedence)(minPrecedence + 1))));
	
			switch (binaryOperator.GetType())
			{
			case Token::Add:
				result.reset(new AddNode(move(operandA), move(operandB)));
				break;
			case Token::Minus:
				result.reset(new SubtractNode(move(operandA), move(operandB)));
				break;
			case Token::Multiply:
				result.reset(new MultiplyNode(move(operandA), move(operandB)));
				break;
			case Token::Divide:
				result.reset(new DivideNode(move(operandA), move(operandB)));
				break;
			case Token::Modulus:
				result.reset(new ModulusNode(move(operandA), move(operandB)));
				break;

			case Token::LessThan:
				result.reset(new LessThanNode(move(operandA), move(operandB)));
				break;
			case Token::LessEqual:
				result.reset(new LessThanEqualNode(move(operandA), move(operandB)));
				break;
			case Token::LeftShift:
				result.reset(new BitshiftLeftNode(move(operandA), move(operandB)));
				break;

			case Token::GreaterThan:
				result.reset(new GreaterThanNode(move(operandA), move(operandB)));
				break;
			case Token::GreaterEqual:
				result.reset(new GreaterThanEqualNode(move(operandA), move(operandB)));
				break;
			case Token::RightShift:
				result.reset(new BitshiftRightNode(move(operandA), move(operandB)));
				break;

			case Token::Equal:
				result.reset(new EqualNode(move(operandA), move(operandB)));
				break;
			case Token::NotEqual:
				result.reset(new NotEqualNode(move(operandA), move(operandB)));
				break;

			case Token::BitAnd:
				result.reset(new BitwiseAndNode(move(operandA), move(operandB)));
				break;
			case Token::BooleanAnd:
				result.reset(new BooleanAndNode(move(operandA), move(operandB)));
				break;

			case Token::BitOr:
				result.reset(new BitwiseOrNode(move(operandA), move(operandB)));
				break;
			case Token::BooleanOr:
				result.reset(new BooleanOrNode(move(operandA), move(operandB)));
				break;

			case Token::BitXor:
				result.reset(new BitwiseXorNode(move(operandA), move(operandB)));
				break;
			default:
				throw Exception(binaryOperator, "Invalid binary operator");
			}
		}
		else
		{
			break;
		}
	}

	return move(result);
}
	
}