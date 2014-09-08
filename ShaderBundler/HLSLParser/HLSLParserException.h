#pragma once

#include "HLSLToken.h"
#include <exception>

class HLSLParserException : public std::exception
{
public:
	HLSLParserException(const std::string& message);
	HLSLParserException(const HLSLToken& token);
	HLSLParserException(const HLSLToken& token, const std::string& message);
	virtual ~HLSLParserException(void) throw();

	virtual const char* what() const throw();
private:
	HLSLToken mToken;
	std::string mMessage;
};

