#include "OpenGLSampler.h"

GLenum OpenGLSampler::gMagFilterMapping[InterpolationMode::Count] = {
	GL_NEAREST,
	GL_LINEAR
};

GLenum OpenGLSampler::gMinMipFilterMapping[InterpolationMode::Count][InterpolationMode::Count] = {
	{GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR},
	{GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR},
};

GLenum OpenGLSampler::gEdgeSampleMapping[EdgeSampling::Count] = {
	GL_REPEAT,
	GL_MIRRORED_REPEAT,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_BORDER,
	GL_MIRROR_CLAMP_TO_EDGE
};

OpenGLSampler::OpenGLSampler(const SamplerDescription& description)
{
	glGenSamplers(1, &mSampler);

	if (description.samplingMode == SamplingMode::Compare)
	{
		glSamplerParameteri(mSampler, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	}
	
	glSamplerParameteri(mSampler, GL_TEXTURE_MIN_FILTER, gMinMipFilterMapping[description.minFilter][description.mipFilter]);
	glSamplerParameteri(mSampler, GL_TEXTURE_MAG_FILTER, gMagFilterMapping[description.magFilter]);

	glSamplerParameteri(mSampler, GL_TEXTURE_WRAP_S, gEdgeSampleMapping[description.uWrap]);
	glSamplerParameteri(mSampler, GL_TEXTURE_WRAP_T, gEdgeSampleMapping[description.vWrap]);
	glSamplerParameteri(mSampler, GL_TEXTURE_WRAP_R, gEdgeSampleMapping[description.wWrap]);

	glSamplerParameterf(mSampler, GL_TEXTURE_LOD_BIAS, description.mipBias);
	glSamplerParameterf(mSampler, GL_TEXTURE_MIN_LOD, description.minLOD);
	glSamplerParameterf(mSampler, GL_TEXTURE_MAX_LOD, description.maxLOD);

	if (description.anisotropicEnabled)
	{
		glSamplerParameterf(mSampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, (float)description.maxAnisotropy);
	}

	glSamplerParameterfv(mSampler, GL_TEXTURE_BORDER_COLOR, reinterpret_cast<const float*>(&description.borderColor));
}


OpenGLSampler::~OpenGLSampler(void)
{
	glDeleteSamplers(1, &mSampler);
}

GLuint OpenGLSampler::GetGLSampler() const
{
	return mSampler;
}