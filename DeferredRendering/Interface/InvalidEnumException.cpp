#include "stdafx.h"
#include "InvalidEnumException.h"

InvalidEnumException::InvalidEnumException(const char *message) : mMessage(message)
{
}


InvalidEnumException::~InvalidEnumException(void)
{
}

std::string InvalidEnumException::BuildWhat() const
{
	return mMessage;
}