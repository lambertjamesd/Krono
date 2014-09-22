#pragma once
#include <exception>
#include <string>

namespace krono
{

class Exception : public std::exception
{
public:
	Exception(void);
	Exception(const char* message);
	Exception(const std::string& message);
	virtual ~Exception(void) throw();

	virtual const char* what() const throw();
protected:
	virtual std::string BuildWhat() const;
private:
	std::string mConstantMessage;
	std::string mWhatMessage;
};

class BadParameterException : public Exception
{
public:
	BadParameterException(const char* message);
};

}
