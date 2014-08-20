
#include "FileNotFoundException.h"

FileNotFoundException::FileNotFoundException(const std::string message) :
	mMessage(message)
{

}

FileNotFoundException::~FileNotFoundException(void) throw()
{

}

const char* FileNotFoundException::what() const throw ()
{
	return mMessage.c_str();
}
