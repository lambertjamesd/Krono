
#include "Exception.h"


Exception::Exception(void)
{
}


Exception::~Exception(void)
{
}

const char* Exception::what() const throw()
{
	if (mWhatMessage.length() == 0)
	{
		const_cast<Exception*>(this)->mWhatMessage = BuildWhat();
	}

	return mWhatMessage.c_str();
}