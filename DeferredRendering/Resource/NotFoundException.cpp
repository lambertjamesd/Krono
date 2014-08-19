#include "NotFoundException.h"

NotFoundException::NotFoundException(const char* message) :
	mMessage(message)
{
}


NotFoundException::~NotFoundException(void)
{
}

std::string NotFoundException::BuildWhat() const
{
	return mMessage;
}