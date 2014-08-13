#pragma once

#include "../Core/Exception.h"

class LoadException : public Exception
{
public:
	LoadException(const std::string& filename, const std::string& message);
	~LoadException(void);

protected:
	virtual std::string BuildWhat() const;
private:
	std::string mFilename;
	std::string mMessage;
};

