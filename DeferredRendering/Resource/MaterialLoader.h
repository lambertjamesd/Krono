#pragma once

#include "ResourceLoader.h"
#include "Material.h"
#include "Technique.h"
#include "JSON/json.h"

namespace krono
{

class MaterialLoader : public ResourceLoader
{
public:
	MaterialLoader(void);
	~MaterialLoader(void);
	
	virtual Auto<Resource> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);

	Material::Ptr ParseMaterial(ResourceManager& resourceManager, const json::Value& root);
	static void ParseTechnique(ResourceManager& resourceManager, Technique& target, const json::Value& root);
private:
};

}