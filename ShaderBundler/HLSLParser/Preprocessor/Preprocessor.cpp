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
	mCurrentToken(0)
{

}

Preprocessor::~Preprocessor(void)
{

}

PreprocessResult Preprocessor::PreprocessFile(const std::string& filename, const std::map<std::string, std::string>& macros)
{
	std::ifstream inputStream(filename, std::ios::binary);
	Preprocessor preprocessor(inputStream);
	
	IncludeHandler includeHandler("", filename);
	MacroStorage storage;

	for (auto it = macros.begin(); it != macros.end(); ++it)
	{
		storage.DefineMacro(it->first, it->second);
	}

	unique_ptr<NodeList> parsedFile(move(preprocessor.ParseFile()));

	PreprocessResult result;
	result.text = GenerateOutputVisitor::Process(*parsedFile, storage, includeHandler);
	result.includedFiles = includeHandler.GetIncludedFiles();
	return result;
}

std::string Preprocessor::PreprocessStream(std::istream& input, MacroStorage& storage, IncludeHandler& includeHandler)
{
	Preprocessor preprocessor(input);
	unique_ptr<NodeList> result(move(preprocessor.ParseFile()));
	return GenerateOutputVisitor::Process(*result, storage, includeHandler);
}

std::unique_ptr<ExpressionNode> Preprocessor::ParseExpression(const std::string& value)
{
	istringstream input(value);
	Preprocessor preprocessor(input);

	const Token& firstToken = preprocessor.PeekNoWhite(0);

	if (firstToken.GetType() == Token::NewLine || firstToken.GetType() == Token::EndOfFile)
	{
		return std::unique_ptr<ExpressionNode>(new ConstantNode(firstToken, 0));
	}
	else
	{
		unique_ptr<ExpressionNode> result(move(preprocessor.ParseBinaryOperator()));

		if (preprocessor.PeekNoWhite(0).GetType() != Token::EndOfFile)
		{
			throw Exception(preprocessor.PeekNoWhite(0), "expected end of expression");
		}

		return move(result);
	}
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
		if (Peek(peekOffset).GetType() != Token::Whitespace)
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
			result->AddNode(std::unique_ptr<Node>(new OtherNode(token)));
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
		return move(ParseDefine(token));
	}
	else if (token.GetValue() == "#error")
	{
		ParseError();
		return std::unique_ptr<DirectiveNode>();
	}
	else if (token.GetValue() == "#if")
	{
		return move(ParseIf(token));
	}
	else if (token.GetValue() == "#ifdef")
	{
		return move(ParseIfDef(token));
	}
	else if (token.GetValue() == "#ifndef")
	{
		return move(ParseIfNDef(token));
	}
	else if (token.GetValue() == "#include")
	{
		return move(ParseInclude(token));
	}
	else if (token.GetValue() == "#line")
	{
		return move(ParseLineDef(token));
	}
	else if (token.GetValue() == "#pragma")
	{
		return move(ParsePragma(token));
	}
	else if (token.GetValue() == "#undef")
	{
		return move(ParseUnDef(token));
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

std::unique_ptr<DefineNode> Preprocessor::ParseDefine(const Token& token)
{
	Require(Token::Whitespace);
	const Token& name = Require(Token::Identifier);
	std::vector<std::string> parameters;

	bool hasParameters = false;

	if (Optional(Token::OpenParen))
	{
		hasParameters = true;

		Optional(Token::Whitespace);

		bool isLooping = !Optional(Token::CloseParen);

		while (isLooping)
		{
			parameters.push_back(RequireNoWhite(Token::Identifier).GetValue());

			if (!OptionalNoWhite(Token::Comma))
			{
				RequireNoWhite(Token::CloseParen);
				isLooping = false;
			}
		}
	}

	Optional(Token::Whitespace);

	std::unique_ptr<NodeList> contents(new NodeList());

	while (!IsEndDirective())
	{
		if (!CheckLineExtend())
		{
			contents->AddNode(move(ParseDefinitionValue()));
		}
	}

	return std::unique_ptr<DefineNode>(new DefineNode(token, name.GetValue(), parameters, hasParameters, move(contents)));
}

std::unique_ptr<IfNode> Preprocessor::ParseIf(const Token& token)
{
	Require(Token::Whitespace);

	unique_ptr<NodeList> expression(new NodeList());

	while (!IsEndDirective())
	{
		if (!CheckLineExtend())
		{
			expression->AddNode(move(ParseDefinitionValue()));
		}
	}

	return move(ParseIfBody(token, move(expression)));
}

std::unique_ptr<IfNode> Preprocessor::ParseIfDef(const Token& ifDefToken)
{
	Require(Token::Whitespace);
	const Token& token = Require(Token::Identifier);
	ConsumeTillEndline();

	string syntax("defined(");
	syntax += token.GetValue();
	syntax += ")";

	return move(ParseIfBody(ifDefToken, std::unique_ptr<OtherNode>(new OtherNode(token, syntax))));
}

std::unique_ptr<IfNode> Preprocessor::ParseIfNDef(const Token& ifNDefToken)
{
	Require(Token::Whitespace);
	const Token& token = Require(Token::Identifier);
	ConsumeTillEndline();

	string syntax("!defined(");
	syntax += token.GetValue();
	syntax += ")";

	return move(ParseIfBody(ifNDefToken, std::unique_ptr<OtherNode>(new OtherNode(token, syntax))));
}

std::unique_ptr<IfNode> Preprocessor::ParseIfBody(const Token& token, std::unique_ptr<Node> expression)
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
		elseBlock = move(ParseIf(nextBlock));
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

	return std::unique_ptr<IfNode>(new IfNode(token, move(expression), move(body), move(elseBlock)));
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

std::unique_ptr<IncludeNode> Preprocessor::ParseInclude(const Token& token)
{
	Require(Token::Whitespace);
	const Token& next = Peek(0);

	if (next.GetType() == Token::String)
	{
		Advance((size_t)1);
		std::string includePath = next.GetValue();

		ConsumeTillEndline();

		return std::unique_ptr<IncludeNode>(new IncludeNode(
			token,
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
			token,
			includePath,
			IncludeNode::SystemInclude));
	}
	else
	{
		throw Exception(next, "Expected include file");
	}
}


std::unique_ptr<LineNode> Preprocessor::ParseLineDef(const Token& token)
{
	Require(Token::Whitespace);
	size_t line = atoi(Require(Token::Other).GetValue().c_str());
	std::string filename;

	if (Optional(Token::Whitespace) && Peek(0).GetType() == Token::String)
	{
		filename = Peek(0).GetValue();
	}

	ConsumeTillEndline();

	return std::unique_ptr<LineNode>(new LineNode(token, line, filename));
}

std::unique_ptr<PragmaNode> Preprocessor::ParsePragma(const Token& token)
{
	while (!IsEndDirective())
	{
		if (!CheckLineExtend())
		{
			Advance((size_t)1);
		}
	}

	return std::unique_ptr<PragmaNode>(new PragmaNode(token));
}

std::unique_ptr<UnDefNode> Preprocessor::ParseUnDef(const Token& token)
{
	Require(Token::Whitespace);

	std::unique_ptr<UnDefNode> result(new UnDefNode(token, Require(Token::Identifier).GetValue()));

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
		std::unique_ptr<Node> result(new OtherNode(Advance()));
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
		return std::unique_ptr<ExpressionNode>(new ConstantNode(next, atoi(next.GetValue().c_str())));
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
	if (PeekNoWhite(1).GetType() == Token::OpenParen)
	{
		return move(ParseFunctionCall(useExpression));
	}
	else
	{
		const Token& name = Require(Token::Identifier, useExpression);

		if (name.GetValue() == "defined" && PeekNoWhite(0).GetType() == Token::Identifier)
		{
			return std::unique_ptr<ExpressionNode>(new DefinedOperatorNode(name, RequireNoWhite(Token::Identifier).GetValue(), false));
		}
		else
		{
			return std::unique_ptr<ExpressionNode>(new IdentifierNode(name));
		}
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
	const Token& name = Require(Token::Identifier);

	if (name.GetValue() == "defined" &&
			PeekNoWhite(0).GetType() == Token::OpenParen &&
			PeekNoWhite(1).GetType() == Token::Identifier &&
			PeekNoWhite(2).GetType() == Token::CloseParen)
	{
		RequireNoWhite(Token::OpenParen);
		std::string identifierName = RequireNoWhite(Token::Identifier).GetValue();
		RequireNoWhite(Token::CloseParen);

		return std::unique_ptr<ExpressionNode>(new DefinedOperatorNode(name, identifierName, true));
	}
	else
	{
		std::unique_ptr<FunctionNode> result(new FunctionNode(name, name.GetValue()));

		Optional(Token::Whitespace);
		Require(Token::OpenParen, useExpression);

		while (!Optional(Token::CloseParen, useExpression))
		{
			result->AddParameter(move(ParseFunctionParameter(useExpression)));
			Optional(Token::Comma, useExpression);
		}
	
		return move(result);
	}
}

std::unique_ptr<ExpressionNode> Preprocessor::ParseUnaryOperator()
{
	if (PeekNoWhite(0).IsUnaryOperator())
	{
		const Token& unaryOperator = AdvanceNoWhite();

		switch (unaryOperator.GetType())
		{
		case Token::BitNot:
			return std::unique_ptr<UnaryOperatorNode>(new BitwiseNotOperatorNode(unaryOperator, move(ParseUnaryOperator())));
		case Token::BooleanNot:
			return std::unique_ptr<UnaryOperatorNode>(new BooleanNotOperatorNode(unaryOperator, move(ParseUnaryOperator())));
		case Token::Minus:
			return std::unique_ptr<UnaryOperatorNode>(new NegateOperatorNode(unaryOperator, move(ParseUnaryOperator())));
		default:
			throw Exception(unaryOperator, " invalid unary operator");
		}
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
		const Token& operatorToken = PeekNoWhite(0);
		Token::OperatorPrecedence precedence = operatorToken.GetPrecedence();

		if (precedence >= minPrecedence)
		{
			const Token& binaryOperator = AdvanceNoWhite();
			std::unique_ptr<ExpressionNode> operandA(move(result));
			std::unique_ptr<ExpressionNode> operandB(move(ParseBinaryOperator((Token::OperatorPrecedence)(minPrecedence + 1))));
	
			switch (binaryOperator.GetType())
			{
			case Token::Add:
				result.reset(new AddNode(operatorToken, move(operandA), move(operandB)));
				break;
			case Token::Minus:
				result.reset(new SubtractNode(operatorToken, move(operandA), move(operandB)));
				break;
			case Token::Multiply:
				result.reset(new MultiplyNode(operatorToken, move(operandA), move(operandB)));
				break;
			case Token::Divide:
				result.reset(new DivideNode(operatorToken, move(operandA), move(operandB)));
				break;
			case Token::Modulus:
				result.reset(new ModulusNode(operatorToken, move(operandA), move(operandB)));
				break;

			case Token::LessThan:
				result.reset(new LessThanNode(operatorToken, move(operandA), move(operandB)));
				break;
			case Token::LessEqual:
				result.reset(new LessThanEqualNode(operatorToken, move(operandA), move(operandB)));
				break;
			case Token::LeftShift:
				result.reset(new BitshiftLeftNode(operatorToken, move(operandA), move(operandB)));
				break;

			case Token::GreaterThan:
				result.reset(new GreaterThanNode(operatorToken, move(operandA), move(operandB)));
				break;
			case Token::GreaterEqual:
				result.reset(new GreaterThanEqualNode(operatorToken, move(operandA), move(operandB)));
				break;
			case Token::RightShift:
				result.reset(new BitshiftRightNode(operatorToken, move(operandA), move(operandB)));
				break;

			case Token::Equal:
				result.reset(new EqualNode(operatorToken, move(operandA), move(operandB)));
				break;
			case Token::NotEqual:
				result.reset(new NotEqualNode(operatorToken, move(operandA), move(operandB)));
				break;

			case Token::BitAnd:
				result.reset(new BitwiseAndNode(operatorToken, move(operandA), move(operandB)));
				break;
			case Token::BooleanAnd:
				result.reset(new BooleanAndNode(operatorToken, move(operandA), move(operandB)));
				break;

			case Token::BitOr:
				result.reset(new BitwiseOrNode(operatorToken, move(operandA), move(operandB)));
				break;
			case Token::BooleanOr:
				result.reset(new BooleanOrNode(operatorToken, move(operandA), move(operandB)));
				break;

			case Token::BitXor:
				result.reset(new BitwiseXorNode(operatorToken, move(operandA), move(operandB)));
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
