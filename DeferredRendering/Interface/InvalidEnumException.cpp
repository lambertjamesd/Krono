
#include "InvalidEnumException.h"

InvalidEnumException::InvalidEnumException(const char *message) : mMessage(message)
{
}


InvalidEnumException::~InvalidEnumException(void) throw()
{
}

std::string InvalidEnumException::BuildWhat() const
{
	return mMessage;
}
