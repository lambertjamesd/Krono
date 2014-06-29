#include "stdafx.h"
#include "OpenGLShaderProgram.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

ShaderVariable::ShaderVariable(const std::string& name, int size, Type type, int count) :
		mName(name),
		mWidth(size),
		mHeight(1),
		mType(type),
		mCount(count)
{

}

ShaderVariable::ShaderVariable(const std::string& name, short width, short height, Type type, int count) :
		mName(name),
		mWidth(width),
		mHeight(height),
		mType(type),
		mCount(count)
{

}

const std::string& ShaderVariable::GetName() const
{
	return mName;
}

short ShaderVariable::GetWidth() const
{
	return mWidth;
}

short ShaderVariable::GetHeight() const
{
	return mHeight;
}

ShaderVariable::Type ShaderVariable::GetType() const
{
	return mType;
}

int ShaderVariable::GetCount() const
{
	return mCount;
}

OpenGLShaderProgram::OpenGLShaderProgram(const OpenGLVertexShader& vertexShader, const OpenGLFragmentShader& fragmentShader)
{
	if (vertexShader.IsValid() && fragmentShader.IsValid())
	{
		vector<GLuint> shaders;
		shaders.push_back(vertexShader.GetGLShader());
		shaders.push_back(fragmentShader.GetGLShader());
		mProgram = LinkProgram(shaders);

		PopulateAttributes();
		PopulateUniforms();
		PopulateOutputs();
	}
	else
	{
		mProgram = 0;
	}
}

OpenGLShaderProgram::~OpenGLShaderProgram()
{

}

void OpenGLShaderProgram::Use()
{
	glUseProgram(mProgram);
}

void OpenGLShaderProgram::BindVertexBuffer(VertexBuffer& vertexBuffer)
{
	// TODO implement
}

void OpenGLShaderProgram::PopulateVariables(std::vector<ShaderVariable>& target, GLenum type)
{
	GLint count;
	glGetProgramInterfaceiv(mProgram, type, GL_ACTIVE_RESOURCES, &count);

	std::vector<GLchar> nameData(256);
	GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_ARRAY_SIZE};
	GLint values[3];

	for (int i = 0; i < count; ++i)
	{
		GLsizei actualAmountRead;
		glGetProgramResourceiv(mProgram, type, i, 
			3, properties, 
			3, &actualAmountRead, values);

		nameData.resize(values[0]);
		glGetProgramResourceName(mProgram, type, i, nameData.size(), NULL, &nameData[0]);
		std::string name((char*)&nameData[0], nameData.size() - 1);

		target.push_back(VariableFromGLType(name, values[1], values[2]));
	}
}

void OpenGLShaderProgram::PopulateAttributes()
{
	PopulateVariables(mAttributes, GL_PROGRAM_INPUT);
}

void OpenGLShaderProgram::PopulateUniforms()
{
	PopulateVariables(mUniforms, GL_UNIFORM);
}

void OpenGLShaderProgram::PopulateOutputs()
{
	PopulateVariables(mOutputs, GL_PROGRAM_OUTPUT);
}

GLuint OpenGLShaderProgram::LinkProgram(const std::vector<GLuint>& shaders)
{
	GLuint result = glCreateProgram();

	for (auto it = shaders.begin(); it != shaders.end(); ++it)
	{
		glAttachShader(result, *it);
	}

	glLinkProgram(result);
	
	for (auto it = shaders.begin(); it != shaders.end(); ++it)
	{
		glDetachShader(result, *it);
	}

	GLint isLinked = 0;
	glGetProgramiv(result, GL_LINK_STATUS, &isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(result, GL_INFO_LOG_LENGTH, &maxLength);
 
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(result, maxLength, &maxLength, &infoLog[0]);
		std::string errorString(infoLog.begin(), infoLog.end());
		std::cerr << "Error linking program source: " << errorString << std::endl;
 
		glDeleteProgram(result);
		result = 0;
	}

	return result;
}

ShaderVariable OpenGLShaderProgram::VariableFromGLType(const std::string& name, GLenum glType, int count)
{
	short width = 0;
	short height = 0;
	ShaderVariable::Type type = ShaderVariable::None;

	switch (glType)
	{
	case GL_FLOAT: width = 1; height = 1; type = ShaderVariable::Float; break;
	case GL_FLOAT_VEC2: width = 2; height = 1; type = ShaderVariable::Float; break;
	case GL_FLOAT_VEC3: width = 3; height = 1; type = ShaderVariable::Float; break;
	case GL_FLOAT_VEC4: width = 4; height = 1; type = ShaderVariable::Float; break;

	case GL_DOUBLE: width = 1; height = 1; type = ShaderVariable::Double; break;
	case GL_DOUBLE_VEC2: width = 2; height = 1; type = ShaderVariable::Double; break;
	case GL_DOUBLE_VEC3: width = 3; height = 1; type = ShaderVariable::Double; break;
	case GL_DOUBLE_VEC4: width = 4; height = 1; type = ShaderVariable::Double; break;

	case GL_INT: width = 1; height = 1; type = ShaderVariable::Int; break;
	case GL_INT_VEC2: width = 2; height = 1; type = ShaderVariable::Int; break;
	case GL_INT_VEC3: width = 3; height = 1; type = ShaderVariable::Int; break;
	case GL_INT_VEC4: width = 4; height = 1; type = ShaderVariable::Int; break;
		
	case GL_FLOAT_MAT2: width = 2; height = 2; type = ShaderVariable::Float; break;
	case GL_FLOAT_MAT3: width = 3; height = 3; type = ShaderVariable::Float; break;
	case GL_FLOAT_MAT4: width = 4; height = 4; type = ShaderVariable::Float; break;
		
	case GL_SAMPLER_1D: width = 1; height = 1; type = ShaderVariable::Texture1D; break;
	case GL_SAMPLER_2D: width = 1; height = 1; type = ShaderVariable::Texture2D; break;
	case GL_SAMPLER_3D: width = 1; height = 1; type = ShaderVariable::Texture3D; break;
	
	}

	return ShaderVariable(name, width, height, type, count);
}

const std::vector<ShaderVariable>& OpenGLShaderProgram::GetAttributes() const
{
	return mAttributes;
}

const std::vector<ShaderVariable>& OpenGLShaderProgram::GetUniforms() const
{
	return mUniforms;
}

const std::vector<ShaderVariable>& OpenGLShaderProgram::GetOutputs() const
{
	return mOutputs;
}
