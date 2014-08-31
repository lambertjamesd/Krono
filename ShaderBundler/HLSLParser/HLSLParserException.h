#pragma once

#include "HLSLToken.h"
#include <exception>

class HLSLParserException : public std::exception
{
public:
	HLSLParserException(const HLSLToken& token);
	virtual ~HLSLParserException(void) throw();

	virtual const char* what() const throw();
private:
	HLSLToken mToken;
	std::string mMessage;
};

