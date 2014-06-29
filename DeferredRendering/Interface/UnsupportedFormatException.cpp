#include "stdafx.h"
#include "UnsupportedFormatException.h"


UnsupportedFormatException::UnsupportedFormatException(const char* message) :
	mMessage(message)
{
}


UnsupportedFormatException::~UnsupportedFormatException(void)
{
}

std::string UnsupportedFormatException::BuildWhat() const
{
	return mMessage;
}
