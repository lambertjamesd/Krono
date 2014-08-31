#include "MaterialLoader.h"
#include "FormatException.h"
#include "JSON/JsonTypeHelper.h"
#include "JSON/JsonDocumentLoader.h"

using namespace json;
using namespace std;

namespace krono
{

MaterialLoader::MaterialLoader(void)
{
}


MaterialLoader::~MaterialLoader(void)
{
}

Auto<Object> MaterialLoader::LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName)
{
	if (!JsonDocumentLoader::IsJsonDocument(inputStream))
	{
		throw FormatException("Material must be a json document");
	}

	Value documentRoot = JsonDocumentLoader::LoadJson(inputStream);

	if (documentRoot["documentType"].ToString("") != "material")
	{
		throw FormatException("documentType not marked as a material");
	}

	return ParseMaterial(resourceManager, documentRoot);
}

Material::Ptr MaterialLoader::ParseMaterial(ResourceManager& resourceManager, const json::Value& root)
{
	Material::Ptr result(new Material());

	if (root.HasKey("techniques") && root["techniques"].GetType() == ArrayVal)
	{
		Array techniques = root["techniques"].ToArray();

		for (auto it = techniques.begin(); it != techniques.end(); ++it)
		{
			Technique technique;
			ParseTechnique(resourceManager, technique, *it);
			result->AddTechnique((*it)["techniqueID"].ToInt(0), technique);
		}
	}

	return result;
}

void MaterialLoader::ParseTechnique(ResourceManager& resourceManager, Technique& target, const json::Value& root)
{
	JsonTypeHelper::ParseRenderStateParameters(resourceManager, target.GetRenderStateParameters(), root);
}

}