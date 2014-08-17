#pragma once

#include "OpenGLCommon.h"

#include "Interface/Shader.h"
#include "OpenGLObjectID.h"

class OpenGLVertexShader : public VertexShader
{
public:
	OpenGLVertexShader(const std::string& source);
	virtual ~OpenGLVertexShader();
	
	static GLuint CompileShader(GLenum type, const std::string& source);

	GLuint GetGLShader() const;
	virtual bool IsValid() const;

	OpenGLObjectID GetObjectID() const;
private:
	OpenGLVertexShader(const OpenGLVertexShader& other);
	OpenGLVertexShader& operator=(const OpenGLVertexShader& other);

	GLuint mShader;

	OpenGLObjectID mObjectID;
};

class OpenGLPixelShader : public PixelShader
{
public:
	OpenGLPixelShader(const std::string& source);
	virtual ~OpenGLPixelShader();
	
	GLuint GetGLShader() const;
	virtual bool IsValid() const;

	OpenGLObjectID GetObjectID() const;
private:
	OpenGLPixelShader(const OpenGLPixelShader& other);
	OpenGLPixelShader& operator=(const OpenGLPixelShader& other);

	GLuint mShader;

	OpenGLObjectID mObjectID;
};