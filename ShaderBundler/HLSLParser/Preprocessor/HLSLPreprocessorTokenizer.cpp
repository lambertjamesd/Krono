#include "HLSLPreprocessorTokenizer.h"
#include <cstring>
#include <sstream>

HLSLPreprocessorToken::HLSLPreprocessorToken(Type type, const std::string& value, size_t line) :
	mType(type),
	mValue(value),
	mLine(line)
{

}

HLSLPreprocessorToken::Type HLSLPreprocessorToken::GetType() const
{
	return mType;
}

const std::string& HLSLPreprocessorToken::GetValue() const
{
	return mValue;
}

size_t HLSLPreprocessorToken::GetLineNumber() const
{
	return mLine;
}

HLSLPreprocessorTokenizer::NextState::NextState(HLSLPreprocessorTokenizer::NextState::StateFunction functionPointer) :
	functionPointer(functionPointer),
	type(HLSLPreprocessorToken::None)
{

}

HLSLPreprocessorTokenizer::NextState::NextState(HLSLPreprocessorTokenizer::NextState::StateFunction functionPointer, HLSLPreprocessorToken::Type tokenType) :
	functionPointer(functionPointer),
	type(tokenType)
{

}

HLSLPreprocessorTokenizer::HLSLPreprocessorTokenizer(std::istream& input)
{
	Tokenize(input);
}

HLSLPreprocessorTokenizer::~HLSLPreprocessorTokenizer(void)
{

}

const HLSLPreprocessorToken& HLSLPreprocessorTokenizer::GetToken(size_t index) const
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

void HLSLPreprocessorTokenizer::Tokenize(std::istream& input)
{
	bool active = true;
	std::ostringstream currentToken;
	size_t lineNumber = 0;

	while (active)
	{
		int nextCharacter = input.get();
		NextState nextState = mCurrentState(nextCharacter);

		if (nextState.type != HLSLPreprocessorToken::None)
		{
			mTokenList.push_back(HLSLPreprocessorToken(nextState.type, currentToken.str(), lineNumber));
			currentToken.str("");

			if (nextState.type == HLSLPreprocessorToken::EndOfFile)
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

HLSLPreprocessorTokenizer::NextState::StateFunction HLSLPreprocessorTokenizer::DefaultState(int nextCharacter)
{
	if (isalpha(nextCharacter) || nextCharacter == '_')
	{
		return &IdentifierState;
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
		return &EndState<HLSLPreprocessorToken::OpenParen>;
	}
	else if (nextCharacter == ')')
	{
		return &EndState<HLSLPreprocessorToken::CloseParen>;
	}
	else if (nextCharacter == '<')
	{
		return &EndState<HLSLPreprocessorToken::OpenAngle>;
	}
	else if (nextCharacter == '>')
	{
		return &EndState<HLSLPreprocessorToken::CloseAngle>;
	}
	else if (nextCharacter == ',')
	{
		return &EndState<HLSLPreprocessorToken::Comma>;
	}
	else if (nextCharacter == '\\')
	{
		return &EndState<HLSLPreprocessorToken::BackSlash>;
	}
	else if (nextCharacter == EOF)
	{
		return &EndState<HLSLPreprocessorToken::EndOfFile>;
	}
	else
	{
		return &OtherState;
	}
}

HLSLPreprocessorTokenizer::NextState HLSLPreprocessorTokenizer::StartState(int nextCharacter)
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

HLSLPreprocessorTokenizer::NextState HLSLPreprocessorTokenizer::LineFeedState(int nextCharacter)
{
	if (nextCharacter == '#')
	{
		return NextState(&DirectiveState, HLSLPreprocessorToken::NewLine);
	}
	else if (nextCharacter == '\r')
	{
		return &NewLineEnd;
	}
	else
	{
		return NextState(DefaultState(nextCharacter), HLSLPreprocessorToken::NewLine);
	}
}

HLSLPreprocessorTokenizer::NextState HLSLPreprocessorTokenizer::CarriageReturnState(int nextCharacter)
{
	if (nextCharacter == '#')
	{
		return NextState(&DirectiveState, HLSLPreprocessorToken::NewLine);
	}
	else if (nextCharacter == '\n')
	{
		return &NewLineEnd;
	}
	else
	{
		return NextState(DefaultState(nextCharacter), HLSLPreprocessorToken::NewLine);
	}
}

HLSLPreprocessorTokenizer::NextState HLSLPreprocessorTokenizer::NewLineEnd(int nextCharacter)
{
	if (nextCharacter == '#')
	{
		return NextState(&DirectiveState, HLSLPreprocessorToken::NewLine);
	}
	else
	{
		return NextState(DefaultState(nextCharacter), HLSLPreprocessorToken::NewLine);
	}
}

HLSLPreprocessorTokenizer::NextState HLSLPreprocessorTokenizer::WhitespaceState(int nextCharacter)
{
	if (isspace(nextCharacter))
	{
		return &WhitespaceState;
	}
	else
	{
		return NextState(DefaultState(nextCharacter), HLSLPreprocessorToken::Whitespace);
	}
}

HLSLPreprocessorTokenizer::NextState HLSLPreprocessorTokenizer::DirectiveState(int nextCharacter)
{
	if (isalpha(nextCharacter))
	{
		return &DirectiveState;
	}
	else
	{
		return NextState(DefaultState(nextCharacter), HLSLPreprocessorToken::Directive);
	}
}

HLSLPreprocessorTokenizer::NextState HLSLPreprocessorTokenizer::IdentifierState(int nextCharacter)
{
	if (isalnum(nextCharacter) || nextCharacter == '_')
	{
		return &IdentifierState;
	}
	else
	{
		return NextState(DefaultState(nextCharacter), HLSLPreprocessorToken::Identifier);
	}
}

HLSLPreprocessorTokenizer::NextState HLSLPreprocessorTokenizer::StringState(int nextCharacter)
{
	if (nextCharacter == '"')
	{
		return NextState(&EndState<HLSLPreprocessorToken::String>);
	}
	else if (nextCharacter == '\\')
	{
		return &StringEscapeState;
	}
	else if (nextCharacter == EOF)
	{
		return NextState(&EndState<HLSLPreprocessorToken::EndOfFile>, HLSLPreprocessorToken::String);
	}
	else if (nextCharacter == '\n' || nextCharacter == '\r')
	{
		return NextState(&EndState<HLSLPreprocessorToken::EndOfFile>, HLSLPreprocessorToken::String);
	}
	else
	{
		return &StringState;
	}
}


HLSLPreprocessorTokenizer::NextState HLSLPreprocessorTokenizer::StringEscapeState(int nextCharacter)
{
	if (nextCharacter == EOF)
	{
		return NextState(&EndState<HLSLPreprocessorToken::EndOfFile>, HLSLPreprocessorToken::String);
	}
	else if (nextCharacter == '\n' || nextCharacter == '\r')
	{
		return NextState(&EndState<HLSLPreprocessorToken::EndOfFile>, HLSLPreprocessorToken::String);
	}
	else
	{
		return &StringState;
	}
}

HLSLPreprocessorTokenizer::NextState HLSLPreprocessorTokenizer::OtherState(int nextCharacter)
{
	NextState::StateFunction nextState = DefaultState(nextCharacter);

	if (nextState != &OtherState)
	{
		return NextState(&IdentifierState, HLSLPreprocessorToken::Other);
	}
	else
	{
		return &OtherState;
	}
}