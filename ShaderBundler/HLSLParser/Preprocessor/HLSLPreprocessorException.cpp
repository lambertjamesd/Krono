#include "HLSLPreprocessorException.h"
#include <sstream>

HLSLPreprocessorException::HLSLPreprocessorException(const HLSLPreprocessorToken& token, const char* expected)
{
	std::ostringstream messageBuilder;
	messageBuilder << "line: " << token.GetLineNumber() << " unexpected token " << token.GetValue() << expected;
	mMessage = messageBuilder.str();
}


HLSLPreprocessorException::~HLSLPreprocessorException(void) throw()
{
}

const char* HLSLPreprocessorException::what() throw()
{
	return mMessage.c_str();
}
