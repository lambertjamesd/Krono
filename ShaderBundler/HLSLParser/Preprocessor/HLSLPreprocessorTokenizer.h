#pragma once

#include <string>
#include <cctype>
#include <vector>
#include <istream>

class HLSLPreprocessorToken
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

		OpenParen,
		CloseParen,

		OpenAngle,
		CloseAngle,

		Comma,

		BackSlash,
	};

	HLSLPreprocessorToken(Type type, const std::string& value, size_t line);

	Type GetType() const;
	const std::string& GetValue() const;
	size_t GetLineNumber() const;
private:
	Type mType;
	std::string mValue;
	size_t mLine;
};

class HLSLPreprocessorTokenizer
{
public:
	HLSLPreprocessorTokenizer(std::istream& input);
	~HLSLPreprocessorTokenizer(void);

	const HLSLPreprocessorToken& GetToken(size_t index) const;
private:
	void Tokenize(std::istream& input);

	struct NextState
	{
		typedef NextState (*StateFunction)(int nextCharacter);

		NextState(StateFunction functionPointer);
		NextState(StateFunction functionPointer, HLSLPreprocessorToken::Type tokenType);
		StateFunction functionPointer;
		HLSLPreprocessorToken::Type type;
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
	static NextState OtherState(int nextCharacter);
	
	template <HLSLPreprocessorToken::Type Type>
	static NextState EndState(int nextCharacter)
	{
		return NextState(DefaultState(nextCharacter), Type);
	}
	
	NextState::StateFunction mCurrentState;
	std::vector<HLSLPreprocessorToken> mTokenList;
};

