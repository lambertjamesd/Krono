#pragma once
#include <exception>
#include <string>

class Exception : public std::exception
{
public:
	Exception(void);
	virtual ~Exception(void);

	virtual const char* what() const throw();
protected:
	virtual std::string BuildWhat() const = 0;
private:
	std::string mWhatMessage;
};

