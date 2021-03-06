#pragma once

#include "Core/Memory.h"
#include "Core/Object.h"
#include "Core/Types.h"
#include "Color.h"
#include "DepthState.h"
#include "Resource/Resource.h"

namespace krono
{

namespace InterpolationMode
{
	enum Type
	{
		Point,
		Linear,

		Count
	};
};

namespace EdgeSampling
{
	enum Type
	{
		Repeat,
		Mirror,
		Clamp,
		Border,
		MirrorOnce,

		Count
	};
};

namespace SamplingMode
{
	enum Type
	{
		Basic,
		Compare,

		Count
	};
};

struct SamplerDescription
{
	SamplerDescription();

	SamplingMode::Type samplingMode;
	bool anisotropicEnabled;

	InterpolationMode::Type minFilter;
	InterpolationMode::Type magFilter;
	InterpolationMode::Type mipFilter;

	EdgeSampling::Type uWrap;
	EdgeSampling::Type vWrap;
	EdgeSampling::Type wWrap;

	bool useMipMap;
	float mipBias;
	float minLOD;
	float maxLOD;

	CompareFunction::Type compareFunction;

	UInt32 maxAnisotropy;

	Colorf borderColor;
};

class Sampler : public Resource
{
public:
	typedef Auto<Sampler> Ptr;

	Sampler(void);
	virtual ~Sampler(void);

	static const Sampler::Ptr Null;
};

}
