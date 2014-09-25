#pragma once

#include "Interface/DataFormat.h"
#include "Compositor/RenderStateParameters.h"
#include "JSON/json.h"
#include "Resource/ResourceManager.h"
#include "Interface/Color.h"
#include "Interface/BlendState.h"
#include "Interface/DepthState.h"
#include "Interface/RasterizerState.h"
#include "Interface/MappedConstantBuffer.h"
#include "Math/Vector4.h"

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
	static Vector4f ParseFloatVector(const json::Value& vector);
	static Vector4i ParseIntVector(const json::Value& vector);

	static CompareFunction::Type ParseCompareFunction(const std::string& value);
	static StencilOperation::Type ParseStencilOperation(const std::string& value);
	static StencilFunction ParseStencilFunction(const json::Value& value);

	static FillMode::Type ParseFillMode(const std::string& value);
	static CullMode::Type ParseCullMode(const std::string& value);

	static RenderTargetBlend ParseRenderTargetBlend(const json::Value& rootValue);
	static BlendState::Ptr ParseBlendState(ResourceManager& resourceManager, const json::Value& blendState);
	static DepthState::Ptr ParseDepthState(ResourceManager& resourceManager, const json::Value& depthState);
	static RasterizerState::Ptr ParseRasterizerState(ResourceManager& resourceManager, const json::Value& rasterizerState);
	static MappedConstantBuffer::Ptr ParseMappedConstantBuffer(ResourceManager& resourceManager, const Shader::Ptr& shader, const json::Value& mappedBuffer);

	static void ParseValueIntoParameters(RenderStateParameters& target, const json::Value& value);
private:
	static const char* gStageName[ShaderStage::TypeCount];
};

}
