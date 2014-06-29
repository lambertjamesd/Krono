#pragma once
#include "..\Core\Exception.h"
#include <string>

class UnsupportedFormatException :
	public Exception
{
public:
	UnsupportedFormatException(const char* message);
	~UnsupportedFormatException(void);
protected:
	virtual std::string BuildWhat() const;
private:
	const char* mMessage;
};
