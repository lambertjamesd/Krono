#include "HLSLTokenizer.h"
#include <cstring>
#include <sstream>
#include <iostream>

HLSLTokenizer::NextState::NextState(HLSLTokenizer::NextState::StateFunction functionPointer) :
	functionPointer(functionPointer),
	type(HLSLTokenType::None)
{

}

HLSLTokenizer::NextState::NextState(HLSLTokenizer::NextState::StateFunction functionPointer, HLSLTokenType::Type type) :
	functionPointer(functionPointer),
	type(type)
{

}

HLSLTokenizer::HLSLTokenizer(std::istream& input) :
	mCurrentState(&StartState)
{
	Tokenize(input);
}


HLSLTokenizer::~HLSLTokenizer(void)
{

}

const HLSLToken& HLSLTokenizer::GetToken(size_t index) const
{
	if (index < mTokenList.size())
	{
		return mTokenList[index];
	}
	else
	{
		return mTokenList.back();
	}
}

const char* HLSLTokenizer::gTokenizerTest = 
	"_id id0 ID 0 1. 1.0 \"string \\\"\"\n"
	"// this is a comment huzah!\n"
	"/* multi \n line \n comment */"
	"+-*/% =+=-=*=/=%="
	"~<<>>&|^<<=>>=&=|=^=&&||?:;,"
	"<> ==!=<=>=++--."
	"[](){} void";

const HLSLToken HLSLTokenizer::gExpectedTokens[] = {
	HLSLToken(HLSLTokenType::Identifier, "_id", 0),
	HLSLToken(HLSLTokenType::Identifier, "id0", 0),
	HLSLToken(HLSLTokenType::Identifier, "ID", 0),

	HLSLToken(HLSLTokenType::Number, "0", 0),
	HLSLToken(HLSLTokenType::Number, "1.", 0),
	HLSLToken(HLSLTokenType::Number, "1.0", 0),
	
	HLSLToken(HLSLTokenType::String, "\"string \\\"\"", 0),
	
	HLSLToken(HLSLTokenType::Add, "+", 4),
	HLSLToken(HLSLTokenType::Minus, "-", 4),
	HLSLToken(HLSLTokenType::Multiply, "*", 4),
	HLSLToken(HLSLTokenType::Divide, "/", 4),
	HLSLToken(HLSLTokenType::Modulus, "%", 4),

	HLSLToken(HLSLTokenType::Assign, "=", 4),
	HLSLToken(HLSLTokenType::AddEqual, "+=", 4),
	HLSLToken(HLSLTokenType::MinusEqual, "-=", 4),
	HLSLToken(HLSLTokenType::MultiplyEqual, "*=", 4),
	HLSLToken(HLSLTokenType::DivideEqual, "/=", 4),
	HLSLToken(HLSLTokenType::ModulusEqual, "%=", 4),
	
	HLSLToken(HLSLTokenType::BitwiseNot, "~", 4),
	HLSLToken(HLSLTokenType::LeftShift, "<<", 4),
	HLSLToken(HLSLTokenType::RightShift, ">>", 4),
	HLSLToken(HLSLTokenType::BitwiseAnd, "&", 4),
	HLSLToken(HLSLTokenType::BitwiseOr, "|", 4),
	HLSLToken(HLSLTokenType::BitwiseXor, "^", 4),

	HLSLToken(HLSLTokenType::LeftShiftEqual, "<<=", 4),
	HLSLToken(HLSLTokenType::RightShiftEqual, ">>=", 4),
	HLSLToken(HLSLTokenType::BitwiseAndEqual, "&=", 4),
	HLSLToken(HLSLTokenType::BitwiseOrEqual, "|=", 4),
	HLSLToken(HLSLTokenType::BitwiseXorEqual, "^=", 4),
	
	HLSLToken(HLSLTokenType::BooleanAnd, "&&", 4),
	HLSLToken(HLSLTokenType::BooleanOr, "||", 4),
	
	HLSLToken(HLSLTokenType::TernaryOperator, "?", 4),
	HLSLToken(HLSLTokenType::Colon, ":", 4),
	HLSLToken(HLSLTokenType::SemiColon, ";", 4),

	HLSLToken(HLSLTokenType::Comma, ",", 4),
	
	HLSLToken(HLSLTokenType::LessThan, "<", 4),
	HLSLToken(HLSLTokenType::GreaterThan, ">", 4),
	HLSLToken(HLSLTokenType::Equal, "==", 4),
	HLSLToken(HLSLTokenType::NotEqual, "!=", 4),
	HLSLToken(HLSLTokenType::LessThanEqual, "<=", 4),
	HLSLToken(HLSLTokenType::GreaterThanEqual, ">=", 4),
	
	HLSLToken(HLSLTokenType::PlusPlus, "++", 4),
	HLSLToken(HLSLTokenType::MinusMinus, "--", 4),
	
	HLSLToken(HLSLTokenType::StructureOperator, ".", 4),
	
	HLSLToken(HLSLTokenType::OpenSquare, "[", 4),
	HLSLToken(HLSLTokenType::CloseSquare, "]", 4),
	HLSLToken(HLSLTokenType::OpenParen, "(", 4),
	HLSLToken(HLSLTokenType::CloseParen, ")", 4),
	HLSLToken(HLSLTokenType::OpenCurly, "{", 4),
	HLSLToken(HLSLTokenType::CloseCurly, "}", 4),

	HLSLToken(HLSLTokenType::Keyword, "void", 4),

	HLSLToken(HLSLTokenType::EndOfFile, "\xff", 4),
};

void HLSLTokenizer::Test()
{
	RunTest(gTokenizerTest, gExpectedTokens, sizeof(gExpectedTokens) / sizeof(gExpectedTokens[0]));
}

void HLSLTokenizer::RunTest(const char* source, const HLSLToken* tokens, size_t tokenCount)
{
	std::istringstream stringStream(source);
	HLSLTokenizer tokenizer(stringStream);

	if (tokenizer.mTokenList.size() != tokenCount)
	{
		std::cerr << "Number of tokens does not match" << std::endl;
	}

	for (size_t i = 0 ; i < tokenCount && i < tokenizer.mTokenList.size(); ++i)
	{
		if (!(tokens[i] == tokenizer.mTokenList[i]))
		{
			std::cerr << "Token mismatch at index " << i << 
				" source=" << tokenizer.mTokenList[i].GetValue() << " expected=" << tokens[i].GetValue() << std::endl;
		}
	}
}

void HLSLTokenizer::Tokenize(std::istream& input)
{
	bool active = true;
	std::ostringstream currentToken;
	size_t lineNumber = 1;

	while (active)
	{
		int nextCharacter = input.get();
		NextState nextState = mCurrentState(nextCharacter);

		if (nextState.type != HLSLTokenType::None)
		{
			HLSLToken newToken(nextState.type, currentToken.str(), lineNumber);

			if (newToken.GetType() != HLSLTokenType::Whitespace && newToken.GetType() != HLSLTokenType::Comment)
			{
				mTokenList.push_back(newToken);
			}

			currentToken.str("");

			if (nextState.type == HLSLTokenType::EndOfFile)
			{
				active = false;
			}
		}

		if (nextCharacter == '\n')
		{
			++lineNumber;
		}

		currentToken << (char)nextCharacter;
		mCurrentState = nextState.functionPointer;
	}
}

HLSLTokenizer::NextState::StateFunction HLSLTokenizer::GetDefaultState(int nextCharacter)
{
	if (isalpha(nextCharacter) || nextCharacter == '_')
	{
		return &Identifier;
	}
	else if (isspace(nextCharacter))
	{
		return &Whitespace;
	}
	else if (isdigit(nextCharacter))
	{
		return &NumberIntegerPart;
	}

	switch (nextCharacter)
	{
	case '"':
		return &String;
	case '+':
		return &Add;
	case '-':
		return &Minus;
	case '*':
		return &Multiply;
	case '/':
		return &Divide;
	case '%':
		return &Modulus;
	case '=':
		return &Assign;
	case '<':
		return &LessThan;
	case '>':
		return &GreaterThan;
	case '&':
		return &BitwiseAnd;
	case '|':
		return &BitwiseOr;
	case '^':
		return &BitwiseXor;
	case '!':
		return &BooleanNot;

	case '~':
		return &EndState<HLSLTokenType::BitwiseNot>;
	case '?':
		return &EndState<HLSLTokenType::TernaryOperator>;
	case ':':
		return &EndState<HLSLTokenType::Colon>;
	case ';':
		return &EndState<HLSLTokenType::SemiColon>;
	case ',':
		return &EndState<HLSLTokenType::Comma>;
	case '.':
		return &EndState<HLSLTokenType::StructureOperator>;

	case '[':
		return &EndState<HLSLTokenType::OpenSquare>;
	case ']':
		return &EndState<HLSLTokenType::CloseSquare>;
	case '(':
		return &EndState<HLSLTokenType::OpenParen>;
	case ')':
		return &EndState<HLSLTokenType::CloseParen>;
	case '{':
		return &EndState<HLSLTokenType::OpenCurly>;
	case '}':
		return &EndState<HLSLTokenType::CloseCurly>;

	case EOF:
		return &EndState<HLSLTokenType::EndOfFile>;
	}

	return &ErrorState;
}
	
HLSLTokenizer::NextState HLSLTokenizer::StartState(int nextCharacter)
{
	return GetDefaultState(nextCharacter);
}

HLSLTokenizer::NextState HLSLTokenizer::Whitespace(int nextCharacter)
{
	if (isspace(nextCharacter))
	{
		return &Whitespace;
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::Whitespace);
	}
}

HLSLTokenizer::NextState HLSLTokenizer::Identifier(int nextCharacter)
{
	if (isalnum(nextCharacter) || nextCharacter == '_')
	{
		return &Identifier;
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::Identifier);
	}
}

HLSLTokenizer::NextState HLSLTokenizer::NumberIntegerPart(int nextCharacter)
{
	if (isdigit(nextCharacter))
	{
		return &NumberIntegerPart;
	}
	else if (nextCharacter == '.')
	{
		return &NumberFractionalPart;
	}
	else if (tolower(nextCharacter) == 'u' || tolower(nextCharacter) == 'l')
	{
		return NextState(&EndState<HLSLTokenType::Number>);
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::Number);
	}
}

HLSLTokenizer::NextState HLSLTokenizer::NumberFractionalPart(int nextCharacter)
{
	if (isdigit(nextCharacter))
	{
		return &NumberFractionalPart;
	}
	else if (tolower(nextCharacter) == 'h' || tolower(nextCharacter) == 'f' || tolower(nextCharacter) == 'l')
	{
		return NextState(&EndState<HLSLTokenType::Number>);
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::Number);
	}
}

HLSLTokenizer::NextState HLSLTokenizer::String(int nextCharacter)
{
	if (nextCharacter == '"')
	{
		return NextState(&EndState<HLSLTokenType::String>);
	}
	else if (nextCharacter == '\\')
	{
		return &StringEscape;
	}
	else if (nextCharacter == EOF)
	{
		return &ErrorState;
	}
	else if (nextCharacter == '\n' || nextCharacter == '\r')
	{
		return NextState(&ErrorState, HLSLTokenType::String);
	}
	else
	{
		return &String;
	}
}


HLSLTokenizer::NextState HLSLTokenizer::StringEscape(int nextCharacter)
{
	if (nextCharacter == EOF)
	{
		return &ErrorState;
	}
	else if (nextCharacter == '\n' || nextCharacter == '\r')
	{
		return NextState(&ErrorState, HLSLTokenType::String);
	}
	else
	{
		return &String;
	}
}

HLSLTokenizer::NextState HLSLTokenizer::Add(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<HLSLTokenType::AddEqual>);
	}
	else if (nextCharacter == '+')
	{
		return NextState(&EndState<HLSLTokenType::PlusPlus>);
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::Add);
	}
}

HLSLTokenizer::NextState HLSLTokenizer::Minus(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<HLSLTokenType::MinusEqual>);
	}
	else if (nextCharacter == '-')
	{
		return NextState(&EndState<HLSLTokenType::MinusMinus>);
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::Minus);
	}
}

HLSLTokenizer::NextState HLSLTokenizer::Multiply(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<HLSLTokenType::MultiplyEqual>);
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::Multiply);
	}
}

HLSLTokenizer::NextState HLSLTokenizer::Divide(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<HLSLTokenType::DivideEqual>);
	}
	else if (nextCharacter == '/')
	{
		return LineComment;
	}
	else if (nextCharacter == '*')
	{
		return MultilineComment;
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::Divide);
	}
}

HLSLTokenizer::NextState HLSLTokenizer::LineComment(int nextCharacter)
{
	if (nextCharacter == '\n' || nextCharacter == '\r' || nextCharacter == EOF)
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::Comment);
	}
	else
	{
		return LineComment;
	}
}

HLSLTokenizer::NextState HLSLTokenizer::MultilineComment(int nextCharacter)
{
	if (nextCharacter == '*')
	{
		return MultilineCommentEnd;
	}
	else
	{
		return MultilineComment;
	}
}

HLSLTokenizer::NextState HLSLTokenizer::MultilineCommentEnd(int nextCharacter)
{
	if (nextCharacter == '/')
	{
		return NextState(&EndState<HLSLTokenType::Comment>);
	}
	else if (nextCharacter == '*')
	{
		return &MultilineCommentEnd;
	}
	else
	{
		return &MultilineComment;
	}
}

HLSLTokenizer::NextState HLSLTokenizer::Modulus(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<HLSLTokenType::ModulusEqual>);
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::Modulus);
	}
}

HLSLTokenizer::NextState HLSLTokenizer::Assign(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<HLSLTokenType::Equal>);
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::Assign);
	}
}

HLSLTokenizer::NextState HLSLTokenizer::LessThan(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<HLSLTokenType::LessThanEqual>);
	}
	else if (nextCharacter == '<')
	{
		return LeftShift;
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::LessThan);
	}
}

HLSLTokenizer::NextState HLSLTokenizer::LeftShift(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<HLSLTokenType::LeftShiftEqual>);
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::LeftShift);
	}
}

HLSLTokenizer::NextState HLSLTokenizer::GreaterThan(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<HLSLTokenType::GreaterThanEqual>);
	}
	else if (nextCharacter == '>')
	{
		return RightShift;
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::GreaterThan);
	}
}

HLSLTokenizer::NextState HLSLTokenizer::RightShift(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<HLSLTokenType::RightShiftEqual>);
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::RightShift);
	}
}

HLSLTokenizer::NextState HLSLTokenizer::BitwiseAnd(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<HLSLTokenType::BitwiseAndEqual>);
	}
	else if (nextCharacter == '&')
	{
		return NextState(&EndState<HLSLTokenType::BooleanAnd>);
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::BitwiseAnd);
	}
}

HLSLTokenizer::NextState HLSLTokenizer::BitwiseOr(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<HLSLTokenType::BitwiseOrEqual>);
	}
	else if (nextCharacter == '|')
	{
		return NextState(&EndState<HLSLTokenType::BooleanOr>);
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::BitwiseOr);
	}
}


HLSLTokenizer::NextState HLSLTokenizer::BitwiseXor(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<HLSLTokenType::BitwiseXorEqual>);
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::BitwiseXor);
	}
}

HLSLTokenizer::NextState HLSLTokenizer::BooleanNot(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<HLSLTokenType::NotEqual>);
	}
	else
	{
		return NextState(GetDefaultState(nextCharacter), HLSLTokenType::BooleanNot);
	}
}


HLSLTokenizer::NextState HLSLTokenizer::ErrorState(int nextCharacter)
{
	return NextState(&EndState<HLSLTokenType::EndOfFile>, HLSLTokenType::Error);
}