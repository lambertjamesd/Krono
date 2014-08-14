#pragma once

#include "Core\Exception.h"

class InvalidEnumException :
	public Exception
{
public:
	InvalidEnumException(const char* message);
	~InvalidEnumException(void);
protected:
	virtual std::string BuildWhat() const;
private:
	const char* mMessage;
};

