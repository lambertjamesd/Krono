#pragma once

#include "Color.h"
#include "Core/Object.h"
#include "Core/Types.h"
#include "Core/Memory.h"

namespace krono
{

namespace Blend
{
	enum Type
	{
		Zero,
		One,
		SrcColor,
		SrcColorInv,
		SrcAlpha,
		SrcAlphaInv,
		DestAlpha,
		DestAlphaInv,
		DestColor,
		DestColorInv,
		SrcAlphaSat,
		BlendFactor,
		BlendFactorInv,
		Src1Color,
		Src1ColorInv,

		Src1Alpha,
		Src1AlphaInv,

		Count
	};
}

namespace BlendOperator
{
	enum Type
	{
		Add,
		Subtract,
		RevSubtract,
		Min,
		Max,

		Count
	};
}

namespace BlendMask
{
	enum Type
	{
		Red = 1 << 0,
		Green = 1 << 1,
		Blue = 1 << 2,
		Alpha = 1 << 3,

		All = Red | Green | Blue | Alpha
	};
}

struct RenderTargetBlend
{
	RenderTargetBlend();

	bool blendEnabled;

	Blend::Type srcBlend;
	Blend::Type destBlend;
	BlendOperator::Type blendOperation;
	
	Blend::Type srcBlendAlpha;
	Blend::Type destBlendAlpha;
	BlendOperator::Type blendOperationAlpha;

	BlendMask::Type blendMask;
};

static const size_t MaxRenderTargetCount = 8;

struct BlendStateDescription
{
	BlendStateDescription();

	bool alphaToCoverage;
	bool independentBlend;
	RenderTargetBlend renderTargets[MaxRenderTargetCount];
	Colorf blendFactor;
	UInt32 sampleMask;
};

class BlendState : public Object
{
public:
	typedef Auto<BlendState> Ptr;

	virtual ~BlendState(void);

	const BlendStateDescription& GetDescription() const;
	void SetBlendFactor(const Colorf& blendFactor);
protected:
	BlendState(const BlendStateDescription& description);

	BlendStateDescription mDescription;
};

}