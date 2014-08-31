#include "HLSLPreprocessor.h"
#include <cstring>
#include "HLSLPreprocessorException.h"

HLSLPreprocessor::HLSLPreprocessor(std::istream& stream, HLSLPreprocessorMacroStorage& definitions) :
	mTokens(stream),
	mDefinitions(definitions)
{

}

HLSLPreprocessor::~HLSLPreprocessor(void)
{

}

std::unique_ptr<HLSLPreprooessorNodeList> HLSLPreprocessor::ParseFile()
{
	const HLSLPreprocessorToken& token = Peek(0);

	std::unique_ptr<HLSLPreprooessorNodeList> result(new HLSLPreprooessorNodeList());

	if (token.GetType() == HLSLPreprocessorToken::Directive)
	{
		result->AddNode(move(ParseDirective()));
	}
	else if (token.GetType() == HLSLPreprocessorToken::Identifier)
	{
		result->AddNode(move(ParseIdentifier()));
	}
	else
	{
		result->AddNode(std::unique_ptr<HLSLPreprocessorNode>(new HLSLPreprocessorOtherNode(token.GetValue())));
	}

	return move(result);
}

std::unique_ptr<HLSLPreprocessorDirectiveNode> HLSLPreprocessor::ParseDirective()
{
	const HLSLPreprocessorToken& token = Require(HLSLPreprocessorToken::Directive);

	if (token.GetValue() == "#define")
	{
		return move(ParseDefine());
	}
	else if (token.GetValue() == "#elif")
	{
		return move(ParseElif());
	}
	else if (token.GetValue() == "#else")
	{
		return move(ParseElse());
	}
	else if (token.GetValue() == "#endif")
	{
		return move(ParseEndIf());
	}
	else if (token.GetValue() == "#error")
	{
		return move(ParseError());
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
	else
	{
		throw HLSLPreprocessorException(token, " invalid preprocessor directive");
	}
}

std::unique_ptr<HLSLPreprocessorDefineNode> HLSLPreprocessor::ParseDefine()
{
	Require(HLSLPreprocessorToken::Whitespace);
	const HLSLPreprocessorToken& name = Require(HLSLPreprocessorToken::Identifier);
	std::vector<std::string> parameters;
	std::unique_ptr<HLSLPreprooessorNodeList> contents;

	if (Optional(HLSLPreprocessorToken::OpenParen))
	{
		Optional(HLSLPreprocessorToken::Whitespace);

		bool isLooping = Optional(HLSLPreprocessorToken::CloseParen);

		while (isLooping)
		{
			Optional(HLSLPreprocessorToken::Whitespace);
			parameters.push_back(Require(HLSLPreprocessorToken::Identifier).GetValue());
			Optional(HLSLPreprocessorToken::Whitespace);

			if (!Optional(HLSLPreprocessorToken::Comma))
			{
				Require(HLSLPreprocessorToken::CloseParen);
				isLooping = false;
			}
		}
	}

	Optional(HLSLPreprocessorToken::Whitespace);

	while (!Optional(HLSLPreprocessorToken::NewLine))
	{
		if (Peek(0).GetType() == HLSLPreprocessorToken::BackSlash &&
			Peek(1).GetType() == HLSLPreprocessorToken::NewLine)
		{
			Skip(2);
		}
		else
		{
			contents->AddNode(move(ParseValue()));
		}
	}

	return std::unique_ptr<HLSLPreprocessorDefineNode>(new HLSLPreprocessorDefineNode(name.GetValue(), parameters, move(contents)));
}

std::unique_ptr<HLSLPreprocessorElifNode> HLSLPreprocessor::ParseElif()
{

}