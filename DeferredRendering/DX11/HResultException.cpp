
#include "HResultException.h"
#include <comdef.h>
#include <DxErr.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "DxErr.lib")

HResultException::HResultException(HRESULT hResult, const char* message) :
		mHResult(hResult),
		mMessage(message)
{
}


HResultException::~HResultException(void)
{
}


std::string HResultException::BuildWhat() const
{
	std::string result(mMessage);

	_com_error comError(mHResult);

	result += " HRESULT: ";

	LPCTSTR comErrorMessage = DXGetErrorString(mHResult);
	size_t length = wcslen(comErrorMessage);
	for (size_t i = 0; i < length; ++i)
	{
		result += (char)comErrorMessage[i];
	}
	
	result += ", ";
	
	LPCTSTR comErrorDescription = DXGetErrorDescription(mHResult);
	length = wcslen(comErrorDescription);
	for (size_t i = 0; i < length; ++i)
	{
		result += (char)comErrorDescription[i];
	}

	return result;
}