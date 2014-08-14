
#include "OpenGLShader.h"
#include <vector>
#include <string>
#include <iostream>

OpenGLVertexShader::OpenGLVertexShader(const std::string& source) :
	mObjectID(OpenGLObjectIDGenerator::GetNextID())
{
	mShader = CompileShader(GL_VERTEX_SHADER, source);
}

OpenGLVertexShader::~OpenGLVertexShader()
{
	glDeleteShader(mShader);
}

GLuint OpenGLVertexShader::GetGLShader() const
{
	return mShader;
}

bool OpenGLVertexShader::IsValid() const
{
	return mShader != 0;
}

GLuint OpenGLVertexShader::CompileShader(GLenum type, const std::string& source)
{
	GLuint result = glCreateShader(type);
	const char* cStrSource = source.c_str();
	GLint sourceLength = source.length();
	glShaderSource(result, 1, &cStrSource, &sourceLength);
	glCompileShader(result);
	GLint isCompiled = 0;
	glGetShaderiv(result, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(result, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(result, maxLength, &maxLength, &errorLog[0]);
        glDeleteShader(result);

		std::string errorString(errorLog.begin(), errorLog.end());
		std::cerr << "Error compiling shader source: " << errorString << std::endl;

		return 0;
	}

	return result;
}

OpenGLObjectID OpenGLVertexShader::GetObjectID() const
{
	return mObjectID;
}

OpenGLPixelShader::OpenGLPixelShader(const std::string& source) :
	mObjectID(OpenGLObjectIDGenerator::GetNextID())
{
	mShader = OpenGLVertexShader::CompileShader(GL_FRAGMENT_SHADER, source);
}

OpenGLPixelShader::~OpenGLPixelShader()
{
	glDeleteShader(mShader);
}

GLuint OpenGLPixelShader::GetGLShader() const
{
	return mShader;
}

bool OpenGLPixelShader::IsValid() const
{
	return mShader != 0;
}

OpenGLObjectID OpenGLPixelShader::GetObjectID() const
{
	return mObjectID;
}
