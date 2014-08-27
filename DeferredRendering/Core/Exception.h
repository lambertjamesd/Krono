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
	virtual ~Exception(void) throw();

	virtual const char* what() const throw();
protected:
	virtual std::string BuildWhat() const;
private:
	const char* mConstantMessage;
	std::string mWhatMessage;
};

class BadParameterException : public Exception
{
public:
	BadParameterException(const char* message);
};

}
