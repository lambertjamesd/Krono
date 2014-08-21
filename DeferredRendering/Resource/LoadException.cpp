
#include "LoadException.h"


LoadException::LoadException(const std::string& filename, const std::string& message) :
	mFilename(filename),
	mMessage(message)
{

}

LoadException::~LoadException(void) throw()
{

}

std::string LoadException::BuildWhat() const
{
	std::string result("Error loading file: ");

	result += mFilename;
	result += " message: ";
	result += mMessage;

	return result;
}
