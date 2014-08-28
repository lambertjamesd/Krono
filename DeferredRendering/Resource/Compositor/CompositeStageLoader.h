#pragma once

#include "Resource/ResourceLoader.h"
#include "Compositor/CompositeStage.h"
#include "JSON/json.h"

namespace krono
{

class CompositeStageLoader : public ResourceLoader
{
public:
	CompositeStageLoader(void);
	~CompositeStageLoader(void);

	virtual Auto<Object> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);

	static CompositeStage::Ptr ParseStage(ResourceManager& resourceManager, const json::Value& stage);
	
private:
	static CompositeStage::Ptr ParseSceneRenderStage(ResourceManager& resourceManager, const json::Value& stage);
	static CompositeStage::Ptr ParseScreenQuadStage(ResourceManager& resourceManager, const json::Value& stage);
};

}