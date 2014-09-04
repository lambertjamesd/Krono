#include "CompositeStageLoader.h"
#include "Resource/FormatException.h"
#include "Resource/JSON/JsonDocumentLoader.h"
#include "Resource/ResourceManager.h"
#include "Resource/JSON/JsonTypeHelper.h"

#include "Compositor/RenderSceneCompositeStage.h"
#include "Compositor/ScreenQuadCompositeStage.h"
#include "Compositor/LightingCompositeStage.h"

#include "Resource/Mesh/Mesh.h"

using namespace json;

namespace krono
{

CompositeStageLoader::CompositeStageLoader(void)
{
}


CompositeStageLoader::~CompositeStageLoader(void)
{
}	

Auto<Object> CompositeStageLoader::LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName)
{
	if (!JsonDocumentLoader::IsJsonDocument(inputStream))
	{
		throw FormatException("CompositeStage must be a json document");
	}

	Value documentRoot = JsonDocumentLoader::LoadJson(inputStream);

	if (documentRoot["documentType"].ToString("") != "compositeStage")
	{
		throw FormatException("documentType not marked as a compositeStage");
	}

	return ParseStage(resourceManager, documentRoot);
}

CompositeStage::Ptr CompositeStageLoader::ParseStage(ResourceManager& resourceManager, const json::Value& stage)
{
	std::string type = stage["type"].ToString("");

	CompositeStage::Ptr result;

	if (type == "sceneRender")
	{
		result = ParseSceneRenderStage(resourceManager, stage);
	}
	else if (type == "screenQuad")
	{
		result = ParseScreenQuadStage(resourceManager, stage);
	}
	else if (type == "lighting")
	{
		result = LightingCompositeStage::Ptr(new LightingCompositeStage(stage["technique"].ToInt(1)));
	}
	else
	{
		throw FormatException("type should be sceneRender or screenQuad");
	}

	result->SetExpectedTargetInputCount(stage["inputCount"].ToInt(0));

	if (stage.HasKey("renderStateParameters"))
	{
		JsonTypeHelper::ParseRenderStateParameters(resourceManager, result->GetStateParameters(), stage["renderStateParameters"]);
	}

	return result;
}

CompositeStage::Ptr CompositeStageLoader::ParseSceneRenderStage(ResourceManager& resourceManager, const json::Value& stage)
{
	RenderSceneCompositeStage::Ptr result(new RenderSceneCompositeStage(stage["technique"].ToInt(0)));

	return result;
}

CompositeStage::Ptr CompositeStageLoader::ParseScreenQuadStage(ResourceManager& resourceManager, const json::Value& stage)
{
	Mesh::Ptr compositeMesh;

	if (stage["compositeMesh"].GetType() == StringVal)
	{
		compositeMesh = resourceManager.LoadResource<Mesh>(stage["compositeMesh"].ToString());
	}
	else
	{
		compositeMesh = resourceManager.GetPlane();
	}

	ScreenQuadCompositeStage::Ptr result(new ScreenQuadCompositeStage(compositeMesh));

	return result;

}

}
