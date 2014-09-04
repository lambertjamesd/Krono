#pragma once

#include <string>
#include <cctype>
#include <vector>
#include <istream>

namespace preproc
{

class Token
{
public:
	enum Type
	{
		None,
		EndOfFile,
		Whitespace,
		Directive,
		NewLine,
		Identifier,
		Other,

		String,

		Number,

		OpenParen,
		CloseParen,

		Comma,

		Add,
		Minus,
		Multiply,
		Divide,
		Modulus,

		LessThan,
		LessEqual,
		LeftShift,

		GreaterThan,
		GreaterEqual,
		RightShift,

		Equal,
		NotEqual,

		BitAnd,
		BooleanAnd,

		BitOr,
		BooleanOr,

		BitXor,
		
		BitNot,
		BooleanNot,

		BackSlash,
	};
	
	enum OperatorPrecedence
	{
		PrecedenceBooleanOr,
		PrecedenceBooleanAnd,
		PrecedenceBitOr,
		PrecedenceBitXor,
		PrecedenceBitAnd,
		PrecedenceEqual,
		PrecedenceCompare,
		PrecedenceBitShift,
		PrecedenceAdd,
		PrecedenceMultiply
	};

	Token(Type type, const std::string& value, size_t line);

	Type GetType() const;
	const std::string& GetValue() const;
	size_t GetLineNumber() const;

	bool IsBinaryOperator() const;
	bool IsUnaryOperator() const;

	OperatorPrecedence GetPrecedence() const;
private:
	Type mType;
	std::string mValue;
	size_t mLine;
};

class Tokenizer
{
public:
	Tokenizer(std::istream& input);
	~Tokenizer(void);

	const Token& GetToken(size_t index) const;
private:
	void Tokenize(std::istream& input);

	struct NextState
	{
		typedef NextState (*StateFunction)(int nextCharacter);

		NextState(StateFunction functionPointer);
		NextState(StateFunction functionPointer, Token::Type tokenType);
		StateFunction functionPointer;
		Token::Type type;
	};
	
	static NextState::StateFunction DefaultState(int nextCharacter);
	static NextState StartState(int nextCharacterv);
	static NextState LineFeedState(int nextCharacter);
	static NextState CarriageReturnState(int nextCharacter);
	static NextState NewLineEnd(int nextCharacter);
	static NextState WhitespaceState(int nextCharacter);
	static NextState DirectiveState(int nextCharacter);
	static NextState IdentifierState(int nextCharacter);
	static NextState StringState(int nextCharacter);
	static NextState StringEscapeState(int nextCharacter);
	static NextState NumberState(int nextCharacter);
	static NextState LessThanState(int nextCharacter);
	static NextState GreaterThanState(int nextCharacter);
	static NextState AndState(int nextCharacter);
	static NextState OrState(int nextCharacter);
	static NextState EqualState(int nextCharacter);
	static NextState BooleanNotState(int nextCharacter);
	static NextState OtherState(int nextCharacter);
	
	template <Token::Type Type>
	static NextState EndState(int nextCharacter)
	{
		return NextState(DefaultState(nextCharacter), Type);
	}
	
	NextState::StateFunction mCurrentState;
	std::vector<Token> mTokenList;
};

}