#pragma once

#include "HLSLToken.h"
#include <istream>
#include <vector>
#include <set>

class HLSLTokenizer
{
public:
	HLSLTokenizer(std::istream& input);
	~HLSLTokenizer(void);

	const HLSLToken& GetToken(size_t index) const;

	static void Test();
private:
	static void RunTest(const char* source, const HLSLToken* tokens, size_t tokenCount);

	static const char* gTokenizerTest;
	static const HLSLToken gExpectedTokens[];

	void Tokenize(std::istream& input);

	struct NextState
	{
		typedef NextState (*StateFunction)(int nextCharacter);

		NextState(StateFunction functionPointer);
		NextState(StateFunction functionPointer, HLSLTokenType::Type type);
		StateFunction functionPointer;
		HLSLTokenType::Type type;
	};

	static NextState::StateFunction GetDefaultState(int nextCharacter);
	
	static NextState StartState(int nextCharacter);
	static NextState Whitespace(int nextCharacter);

	static NextState Identifier(int nextCharacter);

	static NextState NumberIntegerPart(int nextCharacter);
	static NextState NumberFractionalPart(int nextCharacter);

	static NextState String(int nextCharacter);
	static NextState StringEscape(int nextCharacter);

	static NextState Add(int nextCharacter);

	static NextState Minus(int nextCharacter);

	static NextState Multiply(int nextCharacter);

	static NextState Divide(int nextCharacter);

	static NextState LineComment(int nextCharacter);
	static NextState MultilineComment(int nextCharacter);
	static NextState MultilineCommentEnd(int nextCharacter);

	static NextState Modulus(int nextCharacter);

	static NextState Assign(int nextCharacter);

	static NextState LessThan(int nextCharacter);
	static NextState LeftShift(int nextCharacter);

	static NextState GreaterThan(int nextCharacter);
	static NextState RightShift(int nextCharacter);

	static NextState BitwiseAnd(int nextCharacter);

	static NextState BitwiseOr(int nextCharacter);
	
	static NextState BitwiseXor(int nextCharacter);

	static NextState BooleanNot(int nextCharacter);

	static NextState ErrorState(int nextCharacter);

	template <HLSLTokenType::Type Type>
	static NextState EndState(int nextCharacter)
	{
		return NextState(GetDefaultState(nextCharacter), Type);
	}

	NextState::StateFunction mCurrentState;
	std::vector<HLSLToken> mTokenList;
};

