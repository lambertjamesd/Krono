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
	messageBuilder << "error line: " << token.GetLineNumber() << " at '" << token.GetValue() << "' " << expected;
	mMessage = messageBuilder.str();
}


Exception::~Exception(void) throw()
{

}

const char* Exception::what() const throw()
{
	return mMessage.c_str();
}

}
