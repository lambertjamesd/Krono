#pragma once

#include "..\Core\Exception.h"

class FormatException :
	public Exception
{
public:
	FormatException(const char* message);
	~FormatException(void);
protected:
	virtual std::string BuildWhat() const;
private:
	const char* mMessage;
};

