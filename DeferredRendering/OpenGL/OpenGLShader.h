#pragma once

#include "..\Interface\Shader.h"

class OpenGLVertexShader : public VertexShader
{
public:
	OpenGLVertexShader(const std::string& source);
	virtual ~OpenGLVertexShader();
	
	static GLuint CompileShader(GLenum type, const std::string& source);

	GLuint GetGLShader() const;
	virtual bool IsValid() const;
private:
	OpenGLVertexShader(const OpenGLVertexShader& other);
	OpenGLVertexShader& operator=(const OpenGLVertexShader& other);

	GLuint mShader;
};

class OpenGLFragmentShader : public FragmentShader
{
public:
	OpenGLFragmentShader(const std::string& source);
	virtual ~OpenGLFragmentShader();
	
	GLuint GetGLShader() const;
	virtual bool IsValid() const;
private:
	OpenGLFragmentShader(const OpenGLFragmentShader& other);
	OpenGLFragmentShader& operator=(const OpenGLFragmentShader& other);

	GLuint mShader;
};