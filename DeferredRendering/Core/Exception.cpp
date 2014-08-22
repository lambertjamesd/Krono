
#include "Exception.h"


namespace krono
{

Exception::Exception(void)
{
}


Exception::~Exception(void) throw()
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

}
