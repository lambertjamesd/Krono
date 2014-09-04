#include "HLSLParserException.h"

#include <sstream>

HLSLParserException::HLSLParserException(const HLSLToken& token) :
	mToken(token)
{
	std::ostringstream messageBuilder;
	messageBuilder << " line " << token.GetLineNumber() << " unexpected token '" << token.GetValue() << "'";
	mMessage = messageBuilder.str();
}

HLSLParserException::~HLSLParserException(void) throw()
{

}

const char* HLSLParserException::what() const throw()
{
	return mMessage.c_str();
}
