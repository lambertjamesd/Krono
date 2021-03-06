
#include "FormatException.h"

namespace krono
{

FormatException::FormatException(const char* message) :
	mMessage(message)
{
}

FormatException::FormatException(const std::string& message) :
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

}
