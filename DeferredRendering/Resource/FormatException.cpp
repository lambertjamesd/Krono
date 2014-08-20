
#include "FormatException.h"


FormatException::FormatException(const char* message) :
	mMessage(message)
{
}


FormatException::~FormatException(void) throw()
{
}

std::string FormatException::BuildWhat() const
{
	return mMessage;
}
