#pragma once

#include <istream>
#include <map>
#include <string>
#include <functional>
#include <set>

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

struct PreprocessResult
{
	std::string text;
	std::set<std::string> includedFiles;
};

class Preprocessor
{
public:
	~Preprocessor(void);

	static PreprocessResult PreprocessFile(const std::string& filename, const std::map<std::string, std::string>& macros);
	static std::string PreprocessStream(std::istream& input, MacroStorage& storage, IncludeHandler& includeHandler);

	static std::unique_ptr<ExpressionNode> ParseExpression(const std::string& value);

	static void Test();
private:
	Preprocessor(std::istream& stream);

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
	std::unique_ptr<DefineNode> ParseDefine(const Token& token);
	std::unique_ptr<IfNode> ParseIf(const Token& token);
	std::unique_ptr<IfNode> ParseIfDef(const Token& token);
	std::unique_ptr<IfNode> ParseIfNDef(const Token& token);
	
	std::unique_ptr<IfNode> ParseIfBody(const Token& token, std::unique_ptr<Node> expression);

	void ParseError();
	std::unique_ptr<IncludeNode> ParseInclude(const Token& token);
	std::unique_ptr<LineNode> ParseLineDef(const Token& token);
	std::unique_ptr<PragmaNode> ParsePragma(const Token& token);
	std::unique_ptr<UnDefNode> ParseUnDef(const Token& token);
	
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
};

}
