#include "Tokenizer.h"
#include <cstring>
#include <sstream>
#include <cassert>

namespace preproc
{

Token::Token(Type type, const std::string& value, size_t line) :
	mType(type),
	mValue(value),
	mLine(line)
{

}

Token::Type Token::GetType() const
{
	return mType;
}

const std::string& Token::GetValue() const
{
	return mValue;
}

size_t Token::GetLineNumber() const
{
	return mLine;
}

bool Token::IsBinaryOperator() const
{
	return mType >= Add && mType <= BitXor;
}

bool Token::IsUnaryOperator() const
{
	return mType == BitNot || mType == BooleanNot || mType == Minus;
}

Token::OperatorPrecedence Token::GetPrecedence() const
{
	if (mType == Multiply || mType == Divide || mType == Modulus)
	{
		return PrecedenceMultiply;
	}
	else if (mType == Add || mType == Minus)
	{
		return PrecedenceAdd;
	}
	else if (mType == LeftShift || mType == RightShift)
	{
		return PrecedenceBitShift;
	}
	else if (mType == LessThan || mType == LessEqual ||
		mType == GreaterThan || mType == GreaterEqual)
	{
		return PrecedenceCompare;
	}
	else if (mType == Equal || mType == NotEqual)
	{
		return PrecedenceEqual;
	}
	else if (mType == BitAnd)
	{
		return PrecedenceBitAnd;
	}
	else if (mType == BitXor)
	{
		return PrecedenceBitXor;
	}
	else if (mType == BitOr)
	{
		return PrecedenceBitOr;
	}
	else if (mType == BooleanAnd)
	{
		return PrecedenceBooleanAnd;
	}
	else if (mType == BooleanOr)
	{
		return PrecedenceBooleanOr;
	}
	else
	{
		assert(false && "Should never reach here");
		return PrecedenceMultiply;
	}
}

Tokenizer::NextState::NextState(Tokenizer::NextState::StateFunction functionPointer) :
	functionPointer(functionPointer),
	type(Token::None)
{

}

Tokenizer::NextState::NextState(Tokenizer::NextState::StateFunction functionPointer, Token::Type tokenType) :
	functionPointer(functionPointer),
	type(tokenType)
{

}

Tokenizer::Tokenizer(std::istream& input) :
	mCurrentState(&StartState)
{
	Tokenize(input);
}

Tokenizer::~Tokenizer(void)
{

}

const Token& Tokenizer::GetToken(size_t index) const
{
	if (index >= mTokenList.size())
	{
		return mTokenList.back();
	}
	else
	{
		return mTokenList[index];
	}
}

void Tokenizer::Tokenize(std::istream& input)
{
	bool active = true;
	std::ostringstream currentToken;
	size_t lineNumber = 1;

	while (active)
	{
		int nextCharacter = input.get();
		NextState nextState = mCurrentState(nextCharacter);

		if (nextState.type != Token::None)
		{
			mTokenList.push_back(Token(nextState.type, currentToken.str(), lineNumber));
			currentToken.str("");

			if (nextState.type == Token::EndOfFile)
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

Tokenizer::NextState::StateFunction Tokenizer::DefaultState(int nextCharacter)
{
	if (isalpha(nextCharacter) || nextCharacter == '_')
	{
		return &IdentifierState;
	}
	else if (isdigit(nextCharacter))
	{
		return &NumberState;
	}
	else if (nextCharacter == '\n')
	{
		return &LineFeedState;
	}
	else if (nextCharacter == '\r')
	{
		return &CarriageReturnState;
	}
	else if (isspace(nextCharacter))
	{
		return &WhitespaceState;
	}
	else if (nextCharacter == '"')
	{
		return &StringState;
	}
	else if (nextCharacter == '(')
	{
		return &EndState<Token::OpenParen>;
	}
	else if (nextCharacter == ')')
	{
		return &EndState<Token::CloseParen>;
	}
	else if (nextCharacter == ',')
	{
		return &EndState<Token::Comma>;
	}
	else if (nextCharacter == '\\')
	{
		return &EndState<Token::BackSlash>;
	}
	else if (nextCharacter == '+')
	{
		return &EndState<Token::Add>;
	}
	else if (nextCharacter == '-')
	{
		return &EndState<Token::Minus>;
	}
	else if (nextCharacter == '*')
	{
		return &EndState<Token::Multiply>;
	}
	else if (nextCharacter == '/')
	{
		return &EndState<Token::Divide>;
	}
	else if (nextCharacter == '%')
	{
		return &EndState<Token::Modulus>;
	}
	else if (nextCharacter == '<')
	{
		return &LessThanState;
	}
	else if (nextCharacter == '>')
	{
		return &GreaterThanState;
	}
	else if (nextCharacter == '&')
	{
		return &AndState;
	}
	else if (nextCharacter == '|')
	{
		return &OrState;
	}
	else if (nextCharacter == '=')
	{
		return &EqualState;
	}
	else if (nextCharacter == '!')
	{
		return &BooleanNotState;
	}
	else if (nextCharacter == '~')
	{
		return &EndState<Token::BitNot>;
	}
	else if (nextCharacter == EOF)
	{
		return &EndState<Token::EndOfFile>;
	}
	else
	{
		return &OtherState;
	}
}

Tokenizer::NextState Tokenizer::StartState(int nextCharacter)
{
	if (nextCharacter == '#')
	{
		return &DirectiveState;
	}
	else
	{
		return DefaultState(nextCharacter);
	}
}

Tokenizer::NextState Tokenizer::LineFeedState(int nextCharacter)
{
	if (nextCharacter == '#')
	{
		return NextState(&DirectiveState, Token::NewLine);
	}
	else if (nextCharacter == '\r')
	{
		return &NewLineEnd;
	}
	else
	{
		return NextState(DefaultState(nextCharacter), Token::NewLine);
	}
}

Tokenizer::NextState Tokenizer::CarriageReturnState(int nextCharacter)
{
	if (nextCharacter == '#')
	{
		return NextState(&DirectiveState, Token::NewLine);
	}
	else if (nextCharacter == '\n')
	{
		return &NewLineEnd;
	}
	else
	{
		return NextState(DefaultState(nextCharacter), Token::NewLine);
	}
}

Tokenizer::NextState Tokenizer::NewLineEnd(int nextCharacter)
{
	if (nextCharacter == '#')
	{
		return NextState(&DirectiveState, Token::NewLine);
	}
	else
	{
		return NextState(DefaultState(nextCharacter), Token::NewLine);
	}
}

Tokenizer::NextState Tokenizer::WhitespaceState(int nextCharacter)
{
	if (isspace(nextCharacter))
	{
		return &WhitespaceState;
	}
	else
	{
		return NextState(DefaultState(nextCharacter), Token::Whitespace);
	}
}

Tokenizer::NextState Tokenizer::DirectiveState(int nextCharacter)
{
	if (isalpha(nextCharacter))
	{
		return &DirectiveState;
	}
	else
	{
		return NextState(DefaultState(nextCharacter), Token::Directive);
	}
}

Tokenizer::NextState Tokenizer::IdentifierState(int nextCharacter)
{
	if (isalnum(nextCharacter) || nextCharacter == '_')
	{
		return &IdentifierState;
	}
	else
	{
		return NextState(DefaultState(nextCharacter), Token::Identifier);
	}
}

Tokenizer::NextState Tokenizer::StringState(int nextCharacter)
{
	if (nextCharacter == '"')
	{
		return NextState(&EndState<Token::String>);
	}
	else if (nextCharacter == '\\')
	{
		return &StringEscapeState;
	}
	else if (nextCharacter == EOF)
	{
		return NextState(&EndState<Token::EndOfFile>, Token::String);
	}
	else if (nextCharacter == '\n' || nextCharacter == '\r')
	{
		return NextState(&EndState<Token::EndOfFile>, Token::String);
	}
	else
	{
		return &StringState;
	}
}


Tokenizer::NextState Tokenizer::StringEscapeState(int nextCharacter)
{
	if (nextCharacter == EOF)
	{
		return NextState(&EndState<Token::EndOfFile>, Token::String);
	}
	else if (nextCharacter == '\n' || nextCharacter == '\r')
	{
		return NextState(&EndState<Token::EndOfFile>, Token::String);
	}
	else
	{
		return &StringState;
	}
}

Tokenizer::NextState Tokenizer::NumberState(int nextCharacter)
{
	if (isdigit(nextCharacter))
	{
		return &NumberState;
	}
	else
	{
		return NextState(DefaultState(nextCharacter), Token::Number);
	}
}

Tokenizer::NextState Tokenizer::LessThanState(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<Token::LessEqual>);
	}
	else if (nextCharacter == '<')
	{
		return NextState(&EndState<Token::LeftShift>);
	}
	else
	{
		return NextState(DefaultState(nextCharacter), Token::LessThan);
	}
}

Tokenizer::NextState Tokenizer::GreaterThanState(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<Token::GreaterEqual>);
	}
	else if (nextCharacter == '>')
	{
		return NextState(&EndState<Token::RightShift>);
	}
	else
	{
		return NextState(DefaultState(nextCharacter), Token::GreaterThan);
	}
}

Tokenizer::NextState Tokenizer::AndState(int nextCharacter)
{
	if (nextCharacter == '&')
	{
		return NextState(&EndState<Token::BooleanAnd>);
	}
	else
	{
		return NextState(DefaultState(nextCharacter), Token::BitAnd);
	}
}

Tokenizer::NextState Tokenizer::OrState(int nextCharacter)
{
	if (nextCharacter == '|')
	{
		return NextState(&EndState<Token::BooleanOr>);
	}
	else
	{
		return NextState(DefaultState(nextCharacter), Token::BitOr);
	}
}

Tokenizer::NextState Tokenizer::EqualState(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<Token::Equal>);
	}
	else
	{
		return NextState(DefaultState(nextCharacter), Token::Other);
	}
}

Tokenizer::NextState Tokenizer::BooleanNotState(int nextCharacter)
{
	if (nextCharacter == '=')
	{
		return NextState(&EndState<Token::NotEqual>);
	}
	else
	{
		return NextState(DefaultState(nextCharacter), Token::BooleanNot);
	}
}

Tokenizer::NextState Tokenizer::OtherState(int nextCharacter)
{
	NextState::StateFunction nextState = DefaultState(nextCharacter);

	if (nextState != &OtherState)
	{
		return NextState(nextState, Token::Other);
	}
	else
	{
		return &OtherState;
	}
}

}