#pragma once

#include "Core/Exception.h"
#include "OpenGLCommon.h"

namespace krono
{

class OpenGLException: public Exception
{
public:
	OpenGLException(GLenum error, const std::string& message);
	virtual ~OpenGLException(void) throw ();

	static void CheckError(const char* message);
	static void ClearErrorFlag();
private:
};

}
