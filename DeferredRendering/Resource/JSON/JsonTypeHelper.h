#pragma once

#include "Interface/DataFormat.h"
#include "Compositor/RenderStateParameters.h"
#include "JSON/json.h"
#include "Resource/ResourceManager.h"
#include "Interface/Color.h"
#include "Interface/BlendState.h"
#include "Interface/DepthState.h"
#include "Interface/RasterizerState.h"

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

	static RenderTargetBlend ParseRenderTargetBlend(const json::Value& rootValue);
	static BlendState::Ptr ParseBlendState(ResourceManager& resourceManager, const json::Value& blendState);
	static DepthState::Ptr ParseDepthState(ResourceManager& resourceManager, const json::Value& blendState);
	static RasterizerState::Ptr ParseRasterizerState(ResourceManager& resourceManager, const json::Value& blendState);
private:
	static const char* gStageName[ShaderStage::TypeCount];
};

}
