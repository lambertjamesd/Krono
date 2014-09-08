#include "HLSLParserException.h"

#include <sstream>

HLSLParserException::HLSLParserException(const std::string& message) :
	mToken(HLSLTokenType::None, "", 0),
	mMessage(message)
{

}

HLSLParserException::HLSLParserException(const HLSLToken& token) :
	mToken(token)
{
	std::ostringstream messageBuilder;
	messageBuilder << " line " << token.GetLineNumber() << " unexpected token '" << token.GetValue() << "'";
	mMessage = messageBuilder.str();
}

HLSLParserException::HLSLParserException(const HLSLToken& token, const std::string& message) :
	mToken(token)
{
	std::ostringstream messageBuilder;
	messageBuilder << "error at line " << token.GetLineNumber() << " at '" << token.GetValue() << "' " << message;
	mMessage = messageBuilder.str();
}

HLSLParserException::~HLSLParserException(void) throw()
{

}

const char* HLSLParserException::what() const throw()
{
	return mMessage.c_str();
}
