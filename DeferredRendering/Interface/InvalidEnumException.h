#pragma once

#include "Core/Exception.h"

namespace krono
{

class InvalidEnumException :
	public Exception
{
public:
	InvalidEnumException(const char* message);
	~InvalidEnumException(void) throw();
protected:
	virtual std::string BuildWhat() const;
private:
	const char* mMessage;
};

}
