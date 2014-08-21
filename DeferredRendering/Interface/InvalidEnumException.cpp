
#include "InvalidEnumException.h"

namespace krono
{

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

}
