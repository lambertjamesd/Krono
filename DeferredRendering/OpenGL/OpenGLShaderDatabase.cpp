#include "stdafx.h"
#include "OpenGLShaderDatabase.h"
#include "..\Core\Hash.h"

OpenGLShaderDatabase::OpenGLShaderDatabase(void)
{
}


OpenGLShaderDatabase::~OpenGLShaderDatabase(void)
{
}

Auto<OpenGLShaderProgram> OpenGLShaderDatabase::GetProgram(const OpenGLVertexShader& vertexShader, const OpenGLPixelShader& fragmentShader)
{
	HashUInt32 hash;
	OpenGLObjectID vertexID = vertexShader.GetObjectID();
	OpenGLObjectID fragmentID = fragmentShader.GetObjectID();

	hash.Update((void*)(&vertexID), sizeof(vertexID));
	hash.Update((void*)(&fragmentID), sizeof(fragmentID));

	UINT32 signature = hash.GetDigest();

	auto existing = mSavedShaders.find(signature);
	
	Auto<OpenGLShaderProgram> result;

	if (existing == mSavedShaders.end())
	{
		result.reset(new OpenGLShaderProgram(vertexShader, fragmentShader));
		mSavedShaders[signature] = result;
	}
	else
	{
		result = existing->second;
	}

	return result;
}