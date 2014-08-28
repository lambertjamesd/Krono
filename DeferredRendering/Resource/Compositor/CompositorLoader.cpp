#include "CompositorLoader.h"
#include "Resource/FormatException.h"
#include "Resource/JSON/JsonDocumentLoader.h"
#include "Resource/ResourceManager.h"
#include "Resource/JSON/JsonTypeHelper.h"

using namespace json;

namespace krono
{

CompositorLoader::CompositorLoader(void)
{
}


CompositorLoader::~CompositorLoader(void)
{
}
Auto<Object> CompositorLoader::LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName)
{
	if (!JsonDocumentLoader::IsJsonDocument(inputStream))
	{
		throw FormatException("Compositor must be a json document");
	}

	Value documentRoot = JsonDocumentLoader::LoadJson(inputStream);

	if (documentRoot["documentType"].ToString("") != "compositor")
	{
		throw FormatException("documentType not marked as a compositor");
	}

	return ParseCompositor(resourceManager, documentRoot);
}

Compositor::Ptr CompositorLoader::ParseCompositor(ResourceManager& resourceManager, const json::Value& root)
{
	Compositor::Ptr result(new Compositor());

	if (root["renderTargets"].GetType() != ObjectVal)
	{
		throw FormatException("renderTargets must be an object");
	}

	json::Object renderTargets = root["renderTargets"].ToObject();

	ParseRenderTargets(result, renderTargets);

	const Value& stages = root["stages"];

	if (stages.GetType() != ArrayVal)
	{
		throw FormatException("stages must be an array");
	}

	Array arrayStages = stages.ToArray();

	for (auto it = arrayStages.begin(); it != arrayStages.end(); ++it)
	{
		ParseStage(resourceManager, result, *it);
	}

	return result;
}

void CompositorLoader::ParseRenderTargets(Compositor::Ptr& compositor, const json::Object& renderTargets)
{
	for (auto it = renderTargets.begin(); it != renderTargets.end(); ++it)
	{
		const Value& data = it->second;

		DataFormat dataFormat = JsonTypeHelper::ParseDataFormat(data);
		compositor->AddRenderTarget(it->first, dataFormat);

		if (dataFormat.IsDepthFormat())
		{
			if (data.HasKey("clearDepth"))
			{
				compositor->SetClearDepth(it->first, data["clearDepth"].ToFloat(1.0f));
			}
		}
		else
		{
			if (data.HasKey("clearColor"))
			{
				compositor->SetClearColor(it->first, JsonTypeHelper::ParseColor(data["clearColor"]));
			}
		}
	}
}

void CompositorLoader::ParseStage(ResourceManager& resourceManager, Compositor::Ptr& compositor, const json::Value& stageJson)
{
	if (!stageJson.HasKey("stage") || stageJson["stage"].GetType() != StringVal)
	{
		throw FormatException("stages[].stage needs to be a string containing the relvative path to a CompositeStage");
	}

	CompositeStageConnections connections;

	ParseStageConnections(connections, stageJson);

	std::string stageFilename = stageJson["stage"].ToString();
	CompositeStage::Ptr stage = resourceManager.LoadResource<CompositeStage>(stageFilename);
	compositor->AddCompositeStage(stage, connections);
}

void CompositorLoader::ParseStageConnections(CompositeStageConnections& connections, const json::Value& stageJson)
{
	if (stageJson.HasKey("input") && stageJson["input"].GetType() == ArrayVal)
	{
		Array inputArray = stageJson["input"].ToArray();

		for (auto it = inputArray.begin(); it != inputArray.end(); ++it)
		{
			connections.AddRenderTargetInput(it->ToString(""));
		}
	}
	
	if (stageJson.HasKey("output") && stageJson["output"].GetType() == ArrayVal)
	{
		Array inputArray = stageJson["output"].ToArray();

		for (auto it = inputArray.begin(); it != inputArray.end(); ++it)
		{
			connections.AddRenderTarget(it->ToString(""));
		}
	}
	
	if (stageJson.HasKey("depthBuffer") && stageJson["depthBuffer"].GetType() == StringVal)
	{
		connections.SetDepthBuffer(stageJson["depthBuffer"].ToString());
	}
}

}