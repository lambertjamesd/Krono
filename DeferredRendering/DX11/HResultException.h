#pragma once

#include "DX11Common.h"

#include "Core/Exception.h"
#include <string>

class HResultException :
	public Exception
{
public:
	HResultException(HRESULT hResult, const char* message);
	~HResultException(void);
protected:
	virtual std::string BuildWhat() const;
private:
	HRESULT mHResult;
	const char* mMessage;
};

