#pragma once

#include "Core/Types.h"
#include "Core/Memory.h"

namespace krono
{

namespace FillMode
{
	enum Type
	{
		WireFrame,
		Solid,

		Count
	};
}

namespace CullMode
{
	enum Type
	{
		None,
		Front,
		Back,

		Count
	};
}

struct RasterizerStateDescription
{
	RasterizerStateDescription();

	FillMode::Type fillMode;

	CullMode::Type cullMode;
	bool frontCCW;

	UInt32 depthBias;
	float depthBiasClamp;
	float slopeScaledDepthBias;

	bool depthClipEnable;
	bool scissorEnable;
	bool multisampleEnable;
	bool antialiasedLineEnable;
};

class RasterizerState
{
public:
	typedef Auto<RasterizerState> Ptr;

	virtual ~RasterizerState();

	const RasterizerStateDescription& GetDescription() const;
protected:
	RasterizerState(const RasterizerStateDescription& description);
	RasterizerStateDescription mDescription;
private:
	// disable copy constructor and assignment operator
	RasterizerState(const RasterizerState& other);
	RasterizerState& operator=(const RasterizerState& other);

};

}
