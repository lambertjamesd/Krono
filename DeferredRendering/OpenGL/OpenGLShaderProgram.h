#pragma once

#include "OpenGLCommon.h"

#include "Interface/InputLayout.h"
#include "OpenGLShader.h"
#include <map>
#include "OpenGLVertexBuffer.h"
#include "OpenGLTextureStorage.h"
#include "OpenGLConstantBufferStorage.h"

namespace krono
{

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

	ShaderVariable(const std::string& name, int size, Type type, int count, GLuint index);
	ShaderVariable(const std::string& name, short width, short height, Type type, int count, GLuint index);

	const std::string& GetName() const;
	short GetWidth() const;
	short GetHeight() const;

	Type GetType() const;
	int GetCount() const;
	int GetIndex() const;

	bool IsTexture() const;
private:
	std::string mName;
	short mWidth;
	short mHeight;
	Type mType;
	int mCount;
	GLuint mIndex;
};

class OpenGLVertexLayoutData
{
public:
	OpenGLVertexLayoutData(void);
	OpenGLVertexLayoutData(const Attribute& attribute, GLsizei offset);
	~OpenGLVertexLayoutData(void);

	bool GetIsActive() const;
	GLuint GetSize() const;
	GLenum GetType() const;
	GLsizei GetByteSize() const;
	GLsizei GetOffset() const;
private:
	static GLenum gTypeMapping[];

	bool mIsActive;
	GLuint mSize;
	GLenum mType;
	GLsizei mByteSize; 
	GLsizei mOffset;
};

class OpenGLVertexLayout
{
public:
	OpenGLVertexLayout();
	OpenGLVertexLayout(GLsizei stride);
	~OpenGLVertexLayout(void);

	void AddVertexData(const OpenGLVertexLayoutData& value);

	void Use() const;
private:
	std::vector<OpenGLVertexLayoutData> mLayoutData;
	GLsizei mStride;
};

class OpenGLShaderProgram
{
public:
	OpenGLShaderProgram(const OpenGLVertexShader& vertexShader, const OpenGLPixelShader& fragmentShader);
	virtual ~OpenGLShaderProgram();
	
	virtual void Use();

	const std::vector<ShaderVariable>& GetUniforms() const;
	const std::vector<ShaderVariable>& GetOutputs() const;

	void BindVertexBuffer(OpenGLVertexBuffer& buffer);
	void MapTextures(OpenGLTextureStorage& texureStorage) const;
	void MapConstantBuffers(OpenGLConstantBufferStorage& constantStorage) const;
protected:
	
private:
	OpenGLShaderProgram(const OpenGLShaderProgram& other);
	OpenGLShaderProgram& operator=(const OpenGLShaderProgram& other);
	
	void PopulateVariables(std::vector<ShaderVariable>& target, GLenum type);
	void PopulateShaderReferencing(std::vector<size_t>& target, size_t count, GLenum type);
	void PopulateAttributes();
	void PopulateUniforms();
	void PopulateOutputs();
	void PopulateTextureMapping();
	void PopulateConstantBufferMapping();

	static const size_t MIN_SAMPLER_SUFFIX_LENGTH = 2;
	static const char SAMPLER_SUFFIX_CHAR = 's';
	static size_t GetSamplerIndex(const std::string& uniformName);

	const OpenGLVertexLayout& GetLayoutMapping(const InputLayout& inputLayout);

	static GLuint LinkProgram(const std::vector<GLuint>& shaders);
	
	static ShaderVariable VariableFromGLType(const std::string& name, GLenum glType, int count, GLuint index);

	static const size_t MaxAttributeNameLength = 256;

	GLuint mProgram;

	std::map<UInt32, OpenGLVertexLayout> mLayoutMapping;
	
	std::vector<ShaderVariable> mAttributes;
	std::vector<ShaderVariable> mUniforms;
	std::vector<ShaderVariable> mOutputs;

	OpenGLTextureMapping mTextureMapping;
	OpenGLConstantBufferMapping mConstantBufferMapping;
};

}
