#pragma once

#include "HLSLPreprocessorTokenizer.h"
#include <exception>

class HLSLPreprocessorException : public std::exception
{
public:
	HLSLPreprocessorException(const HLSLPreprocessorToken& token, const char* expected);
	virtual ~HLSLPreprocessorException(void) throw();

	virtual const char* what() throw();
private:
	std::string mMessage;
};

