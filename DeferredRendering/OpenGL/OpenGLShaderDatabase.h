#pragma once

#include "Core/Memory.h"

#include "OpenGLShaderProgram.h"
#include <map>

namespace krono
{

class OpenGLShaderDatabase
{
public:
	OpenGLShaderDatabase(void);
	~OpenGLShaderDatabase(void);

	Auto<OpenGLShaderProgram> GetProgram(const OpenGLVertexShader& vertexShader, const OpenGLPixelShader& fragmentShader);
private:
	std::map<UInt32, Auto<OpenGLShaderProgram> > mSavedShaders;
};

}
