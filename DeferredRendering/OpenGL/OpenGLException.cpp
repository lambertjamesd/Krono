#include "OpenGLException.h"

namespace krono
{

OpenGLException::OpenGLException(GLenum error, const std::string& message) :
	Exception(message + std::string(" error: ") + std::string((char*)gluErrorString(error)))
{
}


OpenGLException::~OpenGLException(void)
{
}

void OpenGLException::CheckError(const char* message)
{
	GLenum error = glGetError();

	if (error != GL_NO_ERROR)
	{
		throw OpenGLException(error, message);
	}
}

void OpenGLException::ClearErrorFlag()
{
	glGetError();
}

}