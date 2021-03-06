#pragma once

#include "Core/Exception.h"

namespace krono
{

class LoadException : public Exception
{
public:
	LoadException(const std::string& filename, const std::string& message);
	~LoadException(void) throw();

protected:
	virtual std::string BuildWhat() const;
private:
	std::string mFilename;
	std::string mMessage;
};

}
