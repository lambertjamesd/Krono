#pragma once

#include "Interface/DataFormat.h"
#include "Compositor/RenderStateParameters.h"
#include "JSON/json.h"
#include "Resource/ResourceManager.h"
#include "Interface/Color.h"

namespace krono
{

class JsonTypeHelper
{
public:
	JsonTypeHelper(void);
	~JsonTypeHelper(void);

	static DataFormat ParseDataFormat(const json::Value& data);
	static void ParseRenderStateParameters(ResourceManager& resourceManager, RenderStateParameters& result, const json::Value& stateParameters);
	static Colorf ParseColor(const json::Value& color);
private:
	static const char* gStageName[ShaderStage::TypeCount];
};

}