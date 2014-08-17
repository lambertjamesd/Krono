#include "Sampler.h"
#include <limits>

SamplerDescription::SamplerDescription() :
	samplingMode(SamplingMode::Basic),
	anisotropicEnabled(false),

	minFilter(InterpolationMode::Linear),
	magFilter(InterpolationMode::Linear),
	mipFilter(InterpolationMode::Linear),

	uWrap(EdgeSampling::Clamp),
	vWrap(EdgeSampling::Clamp),
	wWrap(EdgeSampling::Clamp),

	mipBias(0.0f),
	minLOD(-std::numeric_limits<float>::max()),
	maxLOD(std::numeric_limits<float>::max()),
	
	maxAnisotropy(1),

	borderColor(1.0f, 1.0f, 1.0f, 1.0f)
{

}

Sampler::Sampler(void)
{
}


Sampler::~Sampler(void)
{
}
