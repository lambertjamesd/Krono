#pragma once

#include "ResourceLoader.h"
#include "Interface/Sampler.h"
#include "JSON/json.h"

namespace krono
{

class SamplerLoader : public ResourceLoader
{
public:
	SamplerLoader(void);
	~SamplerLoader(void);

	virtual Auto<Resource> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);

	static Sampler::Ptr ParseSampler(ResourceManager& resourceManager, const json::Value& root);
private:
	static InterpolationMode::Type ParseInterpolatioMode(const std::string& value);
	static EdgeSampling::Type ParseEdgeSampling(const std::string& value);
	static SamplingMode::Type ParseSamplingMode(const std::string& value);
	
	static const size_t ShaderInputLayout = 0xFF;
};

}