#pragma once

#include "Core/Types.h"
#include "Core/Memory.h"

namespace krono
{

namespace CompareFunction
{
	enum Type
	{
		Never,
		Less,
		Equal,
		LessEqual,
		Greater,
		GreaterEqual,
		NotEqual,
		Always,

		Count
	};
}

namespace StencilOperation
{
	enum Type
	{
		Keep,
		Zero,
		Replace,
		IncSat,
		DecrSat,
		Invert,
		Incr,
		Decr,

		Count
	};
}

struct StencilFunction
{
	StencilOperation::Type stencilFail;
	StencilOperation::Type stencilDepthFail;
	StencilOperation::Type stencilPass;
	CompareFunction::Type stencilFunction;
};

struct DepthStateDescription
{
	bool depthEnable;
	bool depthWriteEnabled;
	CompareFunction::Type depthCompare;

	bool stencilEnable;
	UInt8 stencilReadMask;
	UInt8 stencilWriteMask;

	StencilFunction frontFace;
	StencilFunction backFace;
};

class DepthState
{
public:
	typedef Auto<DepthState> Ptr;

	~DepthState(void);

	const DepthStateDescription& GetDescription() const;
protected:
	DepthState(const DepthStateDescription& description);

	DepthStateDescription mDescription;
};

}

