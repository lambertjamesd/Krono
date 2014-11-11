#include "SamplerLoader.h"
#include "ResourceManager.h"
#include "FormatException.h"
#include "JSON/JsonTypeHelper.h"
#include "JSON/JsonDocumentLoader.h"

using namespace json;

namespace krono
{

SamplerLoader::SamplerLoader(void)
{
}


SamplerLoader::~SamplerLoader(void)
{
}

Auto<Resource> SamplerLoader::LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName)
{
	if (!JsonDocumentLoader::IsJsonDocument(inputStream))
	{
		throw FormatException("Sampler must be a json document");
	}

	Value documentRoot = JsonDocumentLoader::LoadJson(inputStream);

	if (documentRoot["documentType"].ToString("") != "sampler")
	{
		throw FormatException("documentType not marked as a sampler");
	}

	return ParseSampler(resourceManager, documentRoot);
}

Sampler::Ptr SamplerLoader::ParseSampler(ResourceManager& resourceManager, const json::Value& root)
{
	if (root.GetType() == StringVal)
	{
		return resourceManager.LoadResource<Sampler>(root.ToString());
	}

	SamplerDescription description;

	description.samplingMode = ParseSamplingMode(root["sampling"].ToString("basic"));
	description.anisotropicEnabled = root["anisotropicEnabled"].ToBool(false);

	description.minFilter = ParseInterpolatioMode(root["minFilter"].ToString("linear"));
	description.magFilter = ParseInterpolatioMode(root["magFilter"].ToString("linear"));
	description.mipFilter = ParseInterpolatioMode(root["mipFilter"].ToString("linear"));
	
	description.uWrap = ParseEdgeSampling(root["uWrap"].ToString("clamp"));
	description.vWrap = ParseEdgeSampling(root["vWrap"].ToString("clamp"));
	description.wWrap = ParseEdgeSampling(root["wWrap"].ToString("clamp"));

	description.useMipMap = root["useMipMap"].ToBool(description.useMipMap);

	description.mipBias = root["mipBias"].ToFloat(description.mipBias);
	description.minLOD = root["minLOD"].ToFloat(description.minLOD);
	description.maxLOD = root["maxLOD"].ToFloat(description.maxLOD);

	description.compareFunction = JsonTypeHelper::ParseCompareFunction(root["compareFunction"].ToString("never"));

	description.maxAnisotropy = root["maxAnisotropy"].ToInt(description.maxAnisotropy);

	if (root.HasKey("borderColor"))
	{
		description.borderColor = JsonTypeHelper::ParseColor(root["borderColor"]);
	}

	return resourceManager.GetGraphics()->CreateSampler(description);
}

InterpolationMode::Type SamplerLoader::ParseInterpolatioMode(const std::string& value)
{
	if (value == "point")
	{
		return InterpolationMode::Point;
	}
	else if (value == "linear")
	{
		return InterpolationMode::Linear;
	}
	else
	{
		throw FormatException("interpolation mode must be point or linear");
	}
}

EdgeSampling::Type SamplerLoader::ParseEdgeSampling(const std::string& value)
{
	if (value == "repeat")
	{
		return EdgeSampling::Repeat;
	}
	else if (value == "mirror")
	{
		return EdgeSampling::Mirror;
	}
	else if (value == "clamp")
	{
		return EdgeSampling::Clamp;
	}
	else if (value == "border")
	{
		return EdgeSampling::Border;
	}
	else if (value == "mirrorOnce")
	{
		return EdgeSampling::MirrorOnce;
	}
	else
	{
		throw FormatException("edge sampling mode must be repeat, mirror, clamp, border, mirrorOnce");
	}
}

SamplingMode::Type SamplerLoader::ParseSamplingMode(const std::string& value)
{
	if (value == "basic")
	{
		return SamplingMode::Basic;
	}
	else if (value == "compare")
	{
		return SamplingMode::Compare;
	}
	else
	{
		throw FormatException("sampling mode must be basic or compare");
	}
}

}