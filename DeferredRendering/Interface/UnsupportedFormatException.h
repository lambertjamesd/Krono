#pragma once
#include "Core/Exception.h"
#include <string>

namespace krono
{

class UnsupportedFormatException :
	public Exception
{
public:
	UnsupportedFormatException(const char* message);
	~UnsupportedFormatException(void) throw();
protected:
	virtual std::string BuildWhat() const;
private:
	const char* mMessage;
};

}
