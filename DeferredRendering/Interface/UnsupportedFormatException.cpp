
#include "UnsupportedFormatException.h"

namespace krono
{

UnsupportedFormatException::UnsupportedFormatException(const char* message) :
	mMessage(message)
{
}


UnsupportedFormatException::~UnsupportedFormatException(void) throw()
{
}

std::string UnsupportedFormatException::BuildWhat() const
{
	return mMessage;
}

}

