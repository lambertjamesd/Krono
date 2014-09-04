#pragma once

#include "Tokenizer.h"
#include <exception>

namespace preproc
{

class Exception : public std::exception
{
public:
	Exception(const char* message);
	Exception(const Token& token, const char* expected);
	virtual ~Exception(void) throw();

	virtual const char* what() throw();
private:
	std::string mMessage;
};

}