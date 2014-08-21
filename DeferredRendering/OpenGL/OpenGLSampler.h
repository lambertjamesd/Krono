#pragma once

#include "Interface/Sampler.h"
#include "OpenGLCommon.h"

namespace krono
{

class OpenGLSampler : public Sampler
{
public:
	OpenGLSampler(const SamplerDescription& description);
	~OpenGLSampler(void);

	GLuint GetGLSampler() const;
private:
	GLuint mSampler;

	static GLenum gMagFilterMapping[InterpolationMode::Count];
	static GLenum gMinMipFilterMapping[InterpolationMode::Count][InterpolationMode::Count];
	
	static GLenum gEdgeSampleMapping[EdgeSampling::Count];
};

}
