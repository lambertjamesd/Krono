#pragma once

#include "..\Core\Memory.h"

#include "OpenGLShaderProgram.h"
#include <map>

class OpenGLShaderDatabase
{
public:
	OpenGLShaderDatabase(void);
	~OpenGLShaderDatabase(void);

	Auto<OpenGLShaderProgram> GetProgram(const OpenGLVertexShader& vertexShader, const OpenGLFragmentShader& fragmentShader);
private:
	std::map<UINT32, Auto<OpenGLShaderProgram> > mSavedShaders;
};

