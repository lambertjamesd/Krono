#include "JsonTypeHelper.h"
#include "Resource/FormatException.h"

using namespace json;

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

void JsonTypeHelper::ParseRenderStateParameters(ResourceManager& resourceManager, RenderStateParameters& result, const json::Value& stateParameters)
{
	if (stateParameters.HasKey("vertexShader") && stateParameters["vertexShader"].GetType() == StringVal)
	{
		result.SetVertexShader(resourceManager.LoadResource<VertexShader>(stateParameters["vertexShader"].ToString()));
	}
	
	if (stateParameters.HasKey("pixelShader") && stateParameters["pixelShader"].GetType() == StringVal)
	{
		result.SetPixelShader(resourceManager.LoadResource<PixelShader>(stateParameters["pixelShader"].ToString()));
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

}