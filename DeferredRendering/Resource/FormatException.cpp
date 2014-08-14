
#include "FormatException.h"


FormatException::FormatException(const char* message) :
	mMessage(message)
{
}


FormatException::~FormatException(void)
{
}

std::string FormatException::BuildWhat() const
{
	return mMessage;
}