#pragma once

#include <exception>
#include <string>

class FileNotFoundException : public std::exception
{
public:
	FileNotFoundException(const std::string message);
	virtual ~FileNotFoundException(void) throw();

	virtual const char* what() const throw ();
private:
	std::string mMessage;
};
