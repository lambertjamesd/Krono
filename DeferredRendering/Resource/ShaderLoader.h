#pragma once

#include "ResourceLoader.h"

class VertexShaderLoader : public ResourceLoader
{
public:
	VertexShaderLoader(void);
	~VertexShaderLoader(void);

	virtual Auto<Object> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);
};

class PixelShaderLoader : public ResourceLoader
{
public:
	PixelShaderLoader(void);
	~PixelShaderLoader(void);

	virtual Auto<Object> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);
};