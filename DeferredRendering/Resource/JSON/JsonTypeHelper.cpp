#include "JsonTypeHelper.h"
#include "Resource/FormatException.h"
#include "Resource/SamplerLoader.h"

using namespace json;
using namespace std;

namespace krono
{

JsonTypeHelper::JsonTypeHelper(void)
{
}


JsonTypeHelper::~JsonTypeHelper(void)
{
}

DataFormat JsonTypeHelper::ParseDataFormat(const json::Value& data)
{
	if (data["type"].GetType() != StringVal)
	{
		throw FormatException("Data format needs a string named type");
	}

	std::string typeName = data["type"].ToString();
	DataFormat::Type type;

	if (typeName == "float")
		type = DataFormat::Float;
	else if (typeName == "int8UNorm")
		type = DataFormat::Int8UNorm;
	else if (typeName == "depth16")
		type = DataFormat::Depth16;
	else if (typeName == "depth24")
		type = DataFormat::Depth24;
	else if (typeName == "depth32f")
		type = DataFormat::Depth32F;
	else if (typeName == "depth24s8")
		type = DataFormat::Depth24S8;
	else if (typeName == "depth32fs8")
		type = DataFormat::Depth32FS8;
	else
		throw FormatException("Invalid data format type");

	size_t count = 1;

	if (!DataFormat::IsDepthFormat(type))
	{
		if (!data.HasKey("count") || data["count"].GetType() != IntVal)
		{
			throw FormatException("Data format needs an integer named count");
		}

		count = data["count"].ToInt();
	}

	if (count < 1 || count > 4)
	{
		throw FormatException("Data format count must be in the range [1,4]");
	}

	return DataFormat(type, count);
}

const char* JsonTypeHelper::gStageName[ShaderStage::TypeCount] = {
	"vertexStage",
	"hullStage",
	"domainStage",
	"geometryStage",
	"pixelStage",
	"computeStage"
};

void JsonTypeHelper::ParseRenderStateParameters(ResourceManager& resourceManager, RenderStateParameters& result, const json::Value& stateParameters)
{
	Shader::Ptr shaders[ShaderStage::TypeCount];

	if (stateParameters.HasKey("vertexShader") && stateParameters["vertexShader"].GetType() == StringVal)
	{
		VertexShader::Ptr vertexShader = resourceManager.LoadResource<VertexShader>(stateParameters["vertexShader"].ToString());
		shaders[ShaderStage::VertexShader] = vertexShader;
		result.SetVertexShader(vertexShader);
	}
	
	if (stateParameters.HasKey("pixelShader") && stateParameters["pixelShader"].GetType() == StringVal)
	{
		PixelShader::Ptr pixelShader = resourceManager.LoadResource<PixelShader>(stateParameters["pixelShader"].ToString());
		shaders[ShaderStage::PixelShader] = pixelShader;
		result.SetPixelShader(pixelShader);
	}

	for (size_t i = 0; i < ShaderStage::TypeCount; ++i)
	{
		ShaderStage::Type stageType = static_cast<ShaderStage::Type>(i);

		if (stateParameters.HasKey(gStageName[i]))
		{
			const Value& stage = stateParameters[gStageName[i]];

			if (stage["textures"].GetType() == ArrayVal)
			{
				Array textures = stage["textures"].ToArray();

				for (auto it = textures.begin(); it != textures.end(); ++it)
				{
					if (it->GetType() == StringVal)
					{
						result.AddTexture(resourceManager.LoadResource<Texture>(it->ToString()), stageType);
					}
					else
					{
						result.AddTexture(Texture::Null, stageType);
					}
				}
			}

			if (stage["samplers"].GetType() == ArrayVal)
			{
				Array samplers = stage["samplers"].ToArray();

				for (auto it = samplers.begin(); it != samplers.end(); ++it)
				{
					if (it->GetType() == NULLVal)
					{
						result.AddSampler(Sampler::Null, stageType);
					}
					else
					{
						result.AddSampler(SamplerLoader::ParseSampler(resourceManager, *it), stageType);
					}
				}
			}

			if (stage["uniforms"].GetType() == ArrayVal && shaders[stageType] != NULL)
			{
				Array samplers = stage["uniforms"].ToArray();

				for (auto it = samplers.begin(); it != samplers.end(); ++it)
				{
					result.AddConstantBuffer(ParseMappedConstantBuffer(resourceManager, shaders[i], *it), (ShaderStage::Type)i);
				}
			}
		}
	}

	if (stateParameters.HasKey("blendState"))
	{
		result.SetBlendState(ParseBlendState(resourceManager, stateParameters["blendState"]));
	}

	if (stateParameters.HasKey("depthState"))
	{
		result.SetDepthState(ParseDepthState(resourceManager, stateParameters["depthState"]));
		result.SetStencilReference(stateParameters["stencilReference"].ToInt(0));
	}

	if (stateParameters.HasKey("rasterizerState"))
	{
		result.SetRasterizerState(ParseRasterizerState(resourceManager, stateParameters["rasterizerState"]));
	}
}


Colorf JsonTypeHelper::ParseColor(const json::Value& color)
{
	return Colorf(
		color["r"].ToFloat(0.0f),
		color["g"].ToFloat(0.0f),
		color["b"].ToFloat(0.0f),
		color["a"].ToFloat(1.0f)
		);
}

CompareFunction::Type JsonTypeHelper::ParseCompareFunction(const std::string& value)
{
	if (value == "never")
	{
		return CompareFunction::Never;
	}
	else if (value == "less")
	{
		return CompareFunction::Less;
	}
	else if (value == "equal")
	{
		return CompareFunction::Equal;
	}
	else if (value == "lessEqual")
	{
		return CompareFunction::LessEqual;
	}
	else if (value == "greater")
	{
		return CompareFunction::Greater;
	}
	else if (value == "greaterEqual")
	{
		return CompareFunction::GreaterEqual;
	}
	else if (value == "notEqual")
	{
		return CompareFunction::NotEqual;
	}
	else if (value == "always")
	{
		return CompareFunction::Always;
	}
	else
	{
		throw FormatException("Invalid compare function");
	}
}

StencilOperation::Type JsonTypeHelper::ParseStencilOperation(const std::string& value)
{
	if (value == "keep")
	{
		return StencilOperation::Keep;
	}
	else if (value == "zero")
	{
		return StencilOperation::Zero;
	}
	else if (value == "replace")
	{
		return StencilOperation::Replace;
	}
	else if (value == "incrSat")
	{
		return StencilOperation::IncrSat;
	}
	else if (value == "decrSat")
	{
		return StencilOperation::DecrSat;
	}
	else if (value == "invert")
	{
		return StencilOperation::Invert;
	}
	else if (value == "incr")
	{
		return StencilOperation::Incr;
	}
	else if (value == "decr")
	{
		return StencilOperation::Decr;
	}
	else
	{
		throw FormatException("Invalid stencil operation");
	}
}

StencilFunction JsonTypeHelper::ParseStencilFunction(const json::Value& value)
{
	StencilFunction result;

	if (value.GetType() == ObjectVal)
	{
		result.stencilFail = ParseStencilOperation(value["stencilFail"].ToString("keep"));
		result.stencilDepthFail = ParseStencilOperation(value["stencilDepthFail"].ToString("keep"));
		result.stencilPass = ParseStencilOperation(value["stencilDepthPass"].ToString("keep"));
		result.stencilFunction = ParseCompareFunction(value["stencilFunction"].ToString("always"));
	}
	
	return StencilFunction();
}

FillMode::Type JsonTypeHelper::ParseFillMode(const std::string& value)
{
	if (value == "wireframe")
	{
		return FillMode::WireFrame;
	}
	else if (value == "solid")
	{
		return FillMode::Solid;
	}
	else
	{
		throw FormatException("Invalid fill mode");
	}
}

CullMode::Type JsonTypeHelper::ParseCullMode(const std::string& value)
{
	if (value == "none")
	{
		return CullMode::None;
	}
	else if (value == "back")
	{
		return CullMode::Back;
	}
	else if (value == "front")
	{
		return CullMode::Front;
	}
	else
	{
		throw FormatException("Invalid cull mode");
	}
}

RenderTargetBlend JsonTypeHelper::ParseRenderTargetBlend(const json::Value& rootValue)
{
	if (rootValue.GetType() == StringVal)
	{
		std::string stringValue = rootValue.ToString();

		if (stringValue == "alphaBlend")
		{
			return RenderTargetBlend::AlphaBlend;		
		}
		else if (stringValue == "add")
		{
			return RenderTargetBlend::Add;
		}
		else if (stringValue == "multiply")
		{
			return RenderTargetBlend::Multiply;
		}
		else if (stringValue == "disabled")
		{
			return RenderTargetBlend();
		}
		else
		{
			throw FormatException("Invalid render target blend type");
		}
	}
	else
	{
		RenderTargetBlend result;

		// TODO implement custom blend types

		return result;
	}
}

BlendState::Ptr JsonTypeHelper::ParseBlendState(ResourceManager& resourceManager, const json::Value& blendState)
{
	BlendStateDescription description;

	const Value& renderTargets = blendState["renderTargets"];

	if (renderTargets.GetType() == StringVal)
	{
		description.independentBlend = false;
		description.renderTargets[0] = ParseRenderTargetBlend(renderTargets);
	}
	else if (renderTargets.GetType() == ArrayVal)
	{
		Array arrayValue = renderTargets.ToArray();

		description.independentBlend = true;
		for (size_t i = 0; i < arrayValue.size() && i < MaxRenderTargetCount; ++i)
		{
			description.renderTargets[i] = ParseRenderTargetBlend(arrayValue[i]);
		}
	}
	else
	{
		throw FormatException("renderTargets must be a string or an array");
	}

	if (blendState["blendFactor"].GetType() == ObjectVal)
	{
		description.blendFactor = ParseColor(blendState["blendFactor"]);
	}

	return resourceManager.GetGraphics()->CreateBlendState(description);
}

DepthState::Ptr JsonTypeHelper::ParseDepthState(ResourceManager& resourceManager, const json::Value& depthState)
{
	DepthStateDescription description;
	
	if (depthState.GetType() == StringVal)
	{
		string stringValue = depthState.ToString();

		if (stringValue == "disabled")
		{
			description.depthEnable = false;
		}
		else if (stringValue == "default")
		{
			// use default description, do nothing
		}
		else
		{
			throw FormatException("Unrecognized depthState");
		}
	}
	else
	{
		description.depthEnable = depthState["depthEnable"].ToBool(description.depthEnable);
		description.depthWriteEnabled = depthState["depthWriteEnable"].ToBool(description.depthWriteEnabled);
		description.depthCompare = ParseCompareFunction(depthState["depthCompare"].ToString("less"));
		description.stencilEnable = depthState["stencilEnable"].ToBool(description.depthEnable);
		description.stencilReadMask = depthState["stencilReadMask"].ToInt(description.stencilReadMask);
		description.stencilWriteMask = depthState["stencilWriteMask"].ToInt(description.stencilWriteMask);
		description.frontFace = ParseStencilFunction(depthState["frontFace"]);
		description.backFace = ParseStencilFunction(depthState["backFace"]);
	}

	return resourceManager.GetGraphics()->CreateDepthState(description);
}

RasterizerState::Ptr JsonTypeHelper::ParseRasterizerState(ResourceManager& resourceManager, const json::Value& rasterizerState)
{
	RasterizerStateDescription description;

	if (rasterizerState.GetType() == StringVal)
	{
		string stringValue = rasterizerState.ToString();

		if (stringValue == "default")
		{
			// Use default config, do nothing
		}
		else
		{
			throw FormatException("Unrecognized rasterizer state");
		}
	}
	else
	{
		description.fillMode = ParseFillMode(rasterizerState["fillMode"].ToString("solid"));
		description.cullMode = ParseCullMode(rasterizerState["cullMode"].ToString("back"));
		description.frontCCW = rasterizerState["frontCCW"].ToBool(description.frontCCW);
		description.depthBias = rasterizerState["depthBias"].ToInt(description.depthBias);
		description.depthBiasClamp = rasterizerState["depthBiasClamp"].ToFloat(description.depthBiasClamp);
		description.slopeScaledDepthBias = rasterizerState["slopeScaledDepthBias"].ToFloat(description.slopeScaledDepthBias);
		description.depthClipEnable = rasterizerState["depthClipEnable"].ToBool(description.depthClipEnable);
		description.scissorEnable = rasterizerState["scissorEnable"].ToBool(description.scissorEnable);
		description.multisampleEnable = rasterizerState["multisampleEnable"].ToBool(description.multisampleEnable);
		description.antialiasedLineEnable = rasterizerState["antialiasedLineEnable"].ToBool(description.antialiasedLineEnable);
	}

	return resourceManager.GetGraphics()->CreateRasterizerState(description);
}

MappedConstantBuffer::Ptr JsonTypeHelper::ParseMappedConstantBuffer(ResourceManager& resourceManager, const Shader::Ptr& shader, const json::Value& mappedBuffer)
{
	int registerLocation = mappedBuffer["registerLocation"].ToInt(-1);

	if (registerLocation != -1)
	{
		const ShaderUniformBlockLayout* blockLayout = shader->GetInputLayout().GetUniformBlock(registerLocation);

		if (blockLayout != NULL)
		{
			ConstantBufferLayout layout;
			return MappedConstantBuffer::Ptr(new MappedConstantBuffer(resourceManager.GetGraphics()->CreateConstantBuffer(layout), *blockLayout));
		}
	}
	
	return MappedConstantBuffer::Ptr();
}

}
