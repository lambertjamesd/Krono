
#include "Exception.h"


namespace krono
{

Exception::Exception(void)
{
}

Exception::Exception(const char* message) :
	mConstantMessage(message)
{

}

Exception::Exception(const std::string& message) :
	mConstantMessage(message)
{

}

Exception::~Exception(void) throw()
{
}

std::string Exception::BuildWhat() const
{
	return mConstantMessage;
}

const char* Exception::what() const throw()
{
	if (mWhatMessage.length() == 0)
	{
		const_cast<Exception*>(this)->mWhatMessage = BuildWhat();
	}

	return mWhatMessage.c_str();
}

BadParameterException::BadParameterException(const char* message) :
	Exception(message)
{

}

}
