#pragma once

#include "..\Interface\Shader.h"
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

class OpenGLFragmentShader : public FragmentShader
{
public:
	OpenGLFragmentShader(const std::string& source);
	virtual ~OpenGLFragmentShader();
	
	GLuint GetGLShader() const;
	virtual bool IsValid() const;

	OpenGLObjectID GetObjectID() const;
private:
	OpenGLFragmentShader(const OpenGLFragmentShader& other);
	OpenGLFragmentShader& operator=(const OpenGLFragmentShader& other);

	GLuint mShader;

	OpenGLObjectID mObjectID;
};