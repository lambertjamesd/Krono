#pragma once

#include "Core/Exception.h"

namespace krono
{

class FormatException :
	public Exception
{
public:
	FormatException(const char* message);
	FormatException(const std::string& message);
	~FormatException(void) throw();
protected:
	virtual std::string BuildWhat() const;
private:
	std::string mMessage;
};

}
