#pragma once

#include "Interface/Sampler.h"
#include "DX11SmartPtr.h"
#include "DX11Common.h"

class DX11Sampler : public Sampler
{
public:
	DX11Sampler(ID3D11Device* device, const SamplerDescription& description);
	virtual ~DX11Sampler(void);

	ID3D11SamplerState *GetSamplerState();
private:
	DX11SmartPtr<ID3D11SamplerState> mSamplerState;

	
	static D3D11_TEXTURE_ADDRESS_MODE gAddressModes[EdgeSampling::Count];

	static D3D11_FILTER gAnisotropicFilters[SamplingMode::Count];
	static D3D11_FILTER gRegularFilters[SamplingMode::Count][8];

	static D3D11_FILTER FilterFromDescription(const SamplerDescription& description);
};

