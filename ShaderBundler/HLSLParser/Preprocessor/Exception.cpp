#include "Exception.h"
#include <sstream>

namespace preproc
{

Exception::Exception(const char* message) :
	mMessage(message)
{

}

Exception::Exception(const Token& token, const char* expected)
{
	std::ostringstream messageBuilder;
	messageBuilder << "line: " << token.GetLineNumber() << " unexpected token " << token.GetValue() << expected;
	mMessage = messageBuilder.str();
}


Exception::~Exception(void) throw()
{

}

const char* Exception::what() throw()
{
	return mMessage.c_str();
}

}