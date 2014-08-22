
#include "OpenGLShaderProgram.h"
#include <vector>
#include <string>
#include <iostream>
#include "OpenGLVertexBuffer.h"
#include <sstream>
#include "OpenGLGraphics.h"
#include <cassert>

using namespace std;

namespace krono
{

ShaderVariable::ShaderVariable(const std::string& name, int size, Type type, int count, GLuint index) :
		mName(name),
		mWidth(size),
		mHeight(1),
		mType(type),
		mCount(count),
		mIndex(index)
{

}

ShaderVariable::ShaderVariable(const std::string& name, short width, short height, Type type, int count, GLuint index) :
		mName(name),
		mWidth(width),
		mHeight(height),
		mType(type),
		mCount(count),
		mIndex(index)
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

bool ShaderVariable::IsTexture() const
{
	return mType >= Texture1D && mType <= TextureCube;
}

////////////////////////////////////////////////

GLenum OpenGLVertexLayoutData::gTypeMapping[] = {GL_FLOAT, GL_UNSIGNED_BYTE};

OpenGLVertexLayoutData::OpenGLVertexLayoutData(void) :
	mIsActive(false),
	mSize(0),
	mType(0),
	mByteSize(0),
	mOffset(0)
{

}

OpenGLVertexLayoutData::OpenGLVertexLayoutData(const Attribute& attribute, GLsizei offset) :
	mIsActive(true),
	mSize(attribute.GetFormat().count),
	mType(OpenGLGraphics::GetGLType(attribute.GetFormat().type)),
	mByteSize(attribute.GetFormat().GetSize()),
	mOffset(offset)
{

}

OpenGLVertexLayoutData::~OpenGLVertexLayoutData(void)
{

}

bool OpenGLVertexLayoutData::GetIsActive() const
{
	return mIsActive;
}

GLuint OpenGLVertexLayoutData::GetSize() const
{
	return mSize;
}

GLenum OpenGLVertexLayoutData::GetType() const
{
	return mType;
}

GLsizei OpenGLVertexLayoutData::GetByteSize() const
{
	return mByteSize;
}

GLsizei OpenGLVertexLayoutData::GetOffset() const
{
	return mOffset;
}

//////////////////////////////////////////////////

OpenGLVertexLayout::OpenGLVertexLayout() :
	mStride(0)
{

}

OpenGLVertexLayout::OpenGLVertexLayout(GLsizei stride) :
	mStride(stride)
{

}

OpenGLVertexLayout::~OpenGLVertexLayout(void)
{

}

void OpenGLVertexLayout::AddVertexData(const OpenGLVertexLayoutData& value)
{
	mLayoutData.push_back(value);
}

void OpenGLVertexLayout::Use() const
{
	GLuint index = 0;
	for (auto it = mLayoutData.cbegin(); it != mLayoutData.cend(); ++it, ++index)
	{
		if (it->GetIsActive())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, it->GetSize(), it->GetType(), GL_FALSE, mStride, reinterpret_cast<GLvoid*>(it->GetOffset()));
		}
		else
		{
			glDisableVertexAttribArray(index);
		}
	}
}

/////////////////////////////////////////////

OpenGLShaderProgram::OpenGLShaderProgram(const OpenGLVertexShader& vertexShader, const OpenGLPixelShader& fragmentShader)
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

void OpenGLShaderProgram::BindVertexBuffer(OpenGLVertexBuffer& buffer)
{
	GetLayoutMapping(buffer.GetInputLayout()).Use();
}

void OpenGLShaderProgram::MapTextures(OpenGLTextureStorage& texureStorage) const
{
	texureStorage.UseMapping(mTextureMapping);
}

void OpenGLShaderProgram::MapConstantBuffers(OpenGLConstantBufferStorage& constantStorage) const
{
	constantStorage.UseMapping(mConstantBufferMapping);
}

const OpenGLVertexLayout& OpenGLShaderProgram::GetLayoutMapping(const InputLayout& inputLayout)
{
	auto existingLayout = mLayoutMapping.find(inputLayout.GetSignature());

	if (existingLayout == mLayoutMapping.end())
	{
		OpenGLVertexLayout layoutMapping(inputLayout.GetStride());

		for (auto shaderVar = mAttributes.begin(); shaderVar != mAttributes.end(); ++shaderVar)
		{
			bool foundAttrib = false;
			GLsizei offset = 0;

			for (size_t i = 0; i < inputLayout.GetAttributeCount(); ++i)
			{
				const Attribute& attrib = inputLayout.GetAttribute(i);

				std::ostringstream attrName;

				attrName << "attr" << attrib.GetName() << attrib.GetIndex();

				if (attrName.str() == shaderVar->GetName())
				{
					foundAttrib = true;
					layoutMapping.AddVertexData(OpenGLVertexLayoutData(attrib, offset));
					break;
				}
				else
				{
					offset += attrib.GetFormat().GetSize();
				}
			}

			if (!foundAttrib)
			{
				layoutMapping.AddVertexData(OpenGLVertexLayoutData());
			}
		}

		mLayoutMapping[inputLayout.GetSignature()] = layoutMapping;
		return mLayoutMapping[inputLayout.GetSignature()];
	}
	else
	{
		return existingLayout->second;
	}
}

void OpenGLShaderProgram::PopulateVariables(std::vector<ShaderVariable>& target, GLenum type)
{
	GLint count;
	glGetProgramInterfaceiv(mProgram, type, GL_ACTIVE_RESOURCES, &count);

	GLchar nameData[MaxAttributeNameLength];
	GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_ARRAY_SIZE};
	GLint values[3];

	for (int i = 0; i < count; ++i)
	{
		GLsizei actualAmountRead;
		glGetProgramResourceiv(mProgram, type, i, 
			3, properties, 
			3, &actualAmountRead, values);

		glGetProgramResourceName(mProgram, type, i, values[0] + 1, NULL, nameData);
		std::string name(nameData);

		target.push_back(VariableFromGLType(name, values[1], values[2], i));
	}
}

void OpenGLShaderProgram::PopulateShaderReferencing(std::vector<size_t>& target, size_t count, GLenum type)
{
	GLenum properties[ShaderStage::TypeCount] = {
		GL_REFERENCED_BY_VERTEX_SHADER, 
		GL_REFERENCED_BY_TESS_CONTROL_SHADER, 
		GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
		GL_REFERENCED_BY_GEOMETRY_SHADER,
		GL_REFERENCED_BY_FRAGMENT_SHADER,
		GL_REFERENCED_BY_COMPUTE_SHADER
	};

	GLint values[ShaderStage::TypeCount];

	for (size_t index = 0; index < count; ++index)
	{
		GLsizei actualAmountRead;
		glGetProgramResourceiv(mProgram, type, index, 
			ShaderStage::TypeCount, properties, 
			ShaderStage::TypeCount, &actualAmountRead, values);

		size_t result = 0;
		
		for (size_t stage = 0; stage < ShaderStage::TypeCount; ++stage)
		{
			if (values[stage])
			{
				result |= (1 << stage);
			}
		}

		target.push_back(result);
	}
}

void OpenGLShaderProgram::PopulateAttributes()
{
	PopulateVariables(mAttributes, GL_PROGRAM_INPUT);
}

void OpenGLShaderProgram::PopulateUniforms()
{
	PopulateVariables(mUniforms, GL_UNIFORM);
	PopulateTextureMapping();
}

void OpenGLShaderProgram::PopulateOutputs()
{
	PopulateVariables(mOutputs, GL_PROGRAM_OUTPUT);
}

void OpenGLShaderProgram::PopulateTextureMapping()
{	
	GLenum properties[ShaderStage::TypeCount] = {
		GL_REFERENCED_BY_VERTEX_SHADER, 
		GL_REFERENCED_BY_TESS_CONTROL_SHADER, 
		GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
		GL_REFERENCED_BY_GEOMETRY_SHADER,
		GL_REFERENCED_BY_FRAGMENT_SHADER,
		GL_REFERENCED_BY_COMPUTE_SHADER
	};

	GLint values[ShaderStage::TypeCount];

	size_t textureUnitIndex = 0;

	GLuint previousProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&previousProgram);
	glUseProgram(mProgram);

	for (size_t uniform = 0; uniform < mUniforms.size(); ++uniform)
	{
		if (mUniforms[uniform].IsTexture())
		{
			GLsizei actualAmountRead;
			glGetProgramResourceiv(mProgram, GL_UNIFORM, uniform, 
				ShaderStage::TypeCount, properties, 
				ShaderStage::TypeCount, &actualAmountRead, values);

			ShaderStage::Type shaderStage = ShaderStage::TypeCount;
			
			for (size_t stage = 0; stage < ShaderStage::TypeCount; ++stage)
			{
				if (values[stage])
				{
					shaderStage = (ShaderStage::Type)stage;
					break;
				}
			}

			if (shaderStage != ShaderStage::TypeCount)
			{
				GLuint previousBinding;
				glGetUniformuiv(mProgram, uniform, &previousBinding);
				mTextureMapping.AddTextureUnit(shaderStage, previousBinding, GetSamplerIndex(mUniforms[uniform].GetName()));

				glUniform1ui(uniform, textureUnitIndex);
				++textureUnitIndex;
			}
		}
	}

	glUseProgram(previousProgram);
}

void OpenGLShaderProgram::PopulateConstantBufferMapping()
{
	GLint count;
	glGetProgramInterfaceiv(mProgram, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &count);

	std::vector<size_t> constantBufferUsage;
	PopulateShaderReferencing(constantBufferUsage, count, GL_UNIFORM_BLOCK);

	size_t uniformBlockIndex = 0;
	
	for (size_t uniformBlock = 0; uniformBlock < count; ++uniformBlock)
	{
		ShaderStage::Type shaderStage = ShaderStage::TypeCount;
	
		for (size_t stage = 0; stage < ShaderStage::TypeCount; ++stage)
		{
			if (constantBufferUsage[uniformBlock] & (1 << stage))
			{
				shaderStage = (ShaderStage::Type)stage;
				break;
			}
		}

		if (shaderStage != ShaderStage::TypeCount)
		{
			GLint previousBinding;
			glGetActiveUniformBlockiv(mProgram, uniformBlock, GL_UNIFORM_BLOCK_BINDING, &previousBinding);
			mConstantBufferMapping.AddConstantBuffer(shaderStage, previousBinding);

			glUniformBlockBinding(mProgram, uniformBlock, uniformBlockIndex);
			++uniformBlockIndex;
		}
	}
}

size_t OpenGLShaderProgram::GetSamplerIndex(const std::string& uniformName)
{
	size_t lastUnderscore = uniformName.rfind('_');

	if (lastUnderscore != string::npos && lastUnderscore < uniformName.length() - MIN_SAMPLER_SUFFIX_LENGTH)
	{
		if (tolower(uniformName[lastUnderscore + 1]) == SAMPLER_SUFFIX_CHAR)
		{
			// Need to skip two characters, the underscore and the SAMPLER_SUFFIX_CHAR to get to the number
			return atoi(uniformName.c_str() + lastUnderscore + 2);
		}
	}

	return 0;
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

ShaderVariable OpenGLShaderProgram::VariableFromGLType(const std::string& name, GLenum glType, int count, GLuint index)
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

	return ShaderVariable(name, width, height, type, count, index);
}

const std::vector<ShaderVariable>& OpenGLShaderProgram::GetUniforms() const
{
	return mUniforms;
}

const std::vector<ShaderVariable>& OpenGLShaderProgram::GetOutputs() const
{
	return mOutputs;
}

}
