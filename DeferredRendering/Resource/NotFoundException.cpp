#include "NotFoundException.h"

namespace krono
{

NotFoundException::NotFoundException(const char* message) :
	mMessage(message)
{
}


NotFoundException::~NotFoundException(void) throw ()
{
}

std::string NotFoundException::BuildWhat() const
{
	return mMessage;
}

}
