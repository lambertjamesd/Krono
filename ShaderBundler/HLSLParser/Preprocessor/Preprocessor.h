#pragma once

#include <istream>
#include <map>
#include <string>
#include <functional>

#include "Macro.h"
#include "Tokenizer.h"
#include "ExpressionNode.h"
#include "UnaryOperatorNode.h"
#include "BinaryOperatorNode.h"
#include "Node.h"
#include "DirectiveNode.h"
#include "Exception.h"

#include "IncludeHandler.h"
#include "Macro.h"

namespace preproc
{

class Preprocessor
{
public:
	Preprocessor(std::istream& stream);
	~Preprocessor(void);

	static std::string PreprocessFile(const std::string& filename);
	static std::string PreprocessStream(std::istream& input, MacroStorage& storage, IncludeHandler& includeHandler);
	
	Node& GetRootNode();

	static void Test();
private:
	static void Test(const char* input);

	typedef std::function<bool (const Token&)> TokenChecker;

	size_t GetLocation();
	void RestoreLocation(size_t location);
	
	const Token& PeekNoWhite(size_t offset);
	const Token& RequireNoWhite(Token::Type type);
	void AdvanceNoWhite(size_t count);
	const Token& AdvanceNoWhite();

	bool OptionalNoWhite(Token::Type type);

	const Token& Peek(size_t offset);
	const Token& Require(Token::Type type);
	void Advance(size_t count);
	const Token& Advance();

	bool Optional(Token::Type type);

	const Token& Peek(size_t offset, bool ignoreWhitespace);
	const Token& Require(Token::Type type, bool ignoreWhitespace);
	void Advance(size_t count, bool ignoreWhitespace);
	const Token& Advance(bool ignoreWhitespace);

	bool Optional(Token::Type type, bool ignoreWhitespace);

	bool IsEndDirective();
	
	bool CheckLineExtend();

	void ConsumeTillEndline();

	std::unique_ptr<NodeList> ParseFile();

	std::unique_ptr<NodeList> ParseBlock(TokenChecker shouldContinue, const char* eofMessage);

	std::unique_ptr<DirectiveNode> ParseDirective();
	std::unique_ptr<DefineNode> ParseDefine();
	std::unique_ptr<IfNode> ParseIf();
	std::unique_ptr<IfNode> ParseIfDef();
	std::unique_ptr<IfNode> ParseIfNDef();
	
	std::unique_ptr<IfNode> ParseIfBody(std::unique_ptr<ExpressionNode> expression);

	void ParseError();
	std::unique_ptr<IncludeNode> ParseInclude();
	std::unique_ptr<LineNode> ParseLineDef();
	std::unique_ptr<PragmaNode> ParsePragma();
	std::unique_ptr<UnDefNode> ParseUnDef();
	
	std::unique_ptr<Node> ParseDefinitionValue();
	
	std::unique_ptr<ExpressionNode> ParseValue();
	std::unique_ptr<ExpressionNode> ParseParenthesis();
	std::unique_ptr<ExpressionNode> ParseIdentifier(bool useExpression);
	std::unique_ptr<Node> ParseFunctionParameter(bool useExpression);
	std::unique_ptr<ExpressionNode> ParseFunctionCall(bool useExpression);
	
	std::unique_ptr<ExpressionNode> ParseUnaryOperator();
	std::unique_ptr<ExpressionNode> ParseBinaryOperator(Token::OperatorPrecedence precedence = Token::PrecedenceBooleanOr);
	
	Tokenizer mTokens;
	size_t mCurrentToken;

	std::unique_ptr<Node> mRootNode;
};

}