#pragma once

#include "Core/Exception.h"

namespace krono
{

class NotFoundException :
	public Exception
{
public:
	NotFoundException(const char* message);
	~NotFoundException(void) throw();
protected:
	virtual std::string BuildWhat() const;
private:
	const char* mMessage;
};

}
