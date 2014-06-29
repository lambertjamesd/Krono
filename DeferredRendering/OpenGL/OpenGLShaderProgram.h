#pragma once

#include "..\Interface\ShaderProgram.h"
#include "OpenGLShader.h"

class ShaderVariable
{
public:
	enum Type
	{
		None,
		Float,
		Double,
		Int,
		UInt,
		Bool,
		MatF,
		MatD,
		Texture1D,
		Texture2D,
		Texture3D,
		TextureCube,
	};

	ShaderVariable(const std::string& name, int size, Type type, int count);
	ShaderVariable(const std::string& name, short width, short height, Type type, int count);

	const std::string& GetName() const;
	short GetWidth() const;
	short GetHeight() const;

	Type GetType() const;
	int GetCount() const;
private:
	std::string mName;
	short mWidth;
	short mHeight;
	Type mType;
	int mCount;
};

class OpenGLShaderProgram : public ShaderProgram
{
public:
	OpenGLShaderProgram(const OpenGLVertexShader& vertexShader, const OpenGLFragmentShader& fragmentShader);
	virtual ~OpenGLShaderProgram();
	
	virtual void Use();
	virtual void BindVertexBuffer(VertexBuffer& vertexBuffer);
	
	const std::vector<ShaderVariable>& GetAttributes() const;
	const std::vector<ShaderVariable>& GetUniforms() const;
	const std::vector<ShaderVariable>& GetOutputs() const;
protected:
	
private:
	OpenGLShaderProgram(const OpenGLShaderProgram& other);
	OpenGLShaderProgram& operator=(const OpenGLShaderProgram& other);

	void PopulateAttributes();
	void PopulateUniforms();
	void PopulateOutputs();
	void PopulateVariables(std::vector<ShaderVariable>& target, GLenum type);

	static GLuint LinkProgram(const std::vector<GLuint>& shaders);
	
	static ShaderVariable VariableFromGLType(const std::string& name, GLenum glType, int count);

	GLuint mProgram;
	
	std::vector<ShaderVariable> mAttributes;
	std::vector<ShaderVariable> mUniforms;
	std::vector<ShaderVariable> mOutputs;
};