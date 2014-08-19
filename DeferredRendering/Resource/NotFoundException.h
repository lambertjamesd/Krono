#pragma once

#include "Core/Exception.h"

class NotFoundException :
	public Exception
{
public:
	NotFoundException(const char* message);
	~NotFoundException(void);
protected:
	virtual std::string BuildWhat() const;
private:
	const char* mMessage;
};

