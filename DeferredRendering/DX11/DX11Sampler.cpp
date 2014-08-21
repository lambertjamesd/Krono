#include "DX11Sampler.h"

#include "HResultException.h"

	
D3D11_TEXTURE_ADDRESS_MODE DX11Sampler::gAddressModes[EdgeSampling::Count] = {
	D3D11_TEXTURE_ADDRESS_WRAP,
	D3D11_TEXTURE_ADDRESS_MIRROR,
	D3D11_TEXTURE_ADDRESS_CLAMP,
	D3D11_TEXTURE_ADDRESS_BORDER,
	D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
};

DX11Sampler::DX11Sampler(ID3D11Device* device, const SamplerDescription& description)
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = FilterFromDescription(description);
	samplerDesc.AddressU = gAddressModes[description.uWrap];
	samplerDesc.AddressV = gAddressModes[description.vWrap];
	samplerDesc.AddressW = gAddressModes[description.wWrap];
	samplerDesc.MipLODBias = description.mipBias;
	samplerDesc.MaxAnisotropy = description.maxAnisotropy;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = description.borderColor.r;
	samplerDesc.BorderColor[1] = description.borderColor.g;
	samplerDesc.BorderColor[2] = description.borderColor.b;
	samplerDesc.BorderColor[3] = description.borderColor.a;
	samplerDesc.MinLOD = description.minLOD;
	samplerDesc.MaxLOD = description.maxLOD;

	HRESULT result = device->CreateSamplerState(&samplerDesc, &mSamplerState);

	if (FAILED(result))
	{
		throw HResultException(result, "Could not create sampler");
	}
}


DX11Sampler::~DX11Sampler(void)
{
}

ID3D11SamplerState *DX11Sampler::GetSamplerState()
{
	return mSamplerState.Get();
}

D3D11_FILTER DX11Sampler::gAnisotropicFilters[SamplingMode::Count] = {
	D3D11_FILTER_ANISOTROPIC,
	D3D11_FILTER_COMPARISON_ANISOTROPIC,
};

D3D11_FILTER DX11Sampler::gRegularFilters[SamplingMode::Count][8] = {
	{
		D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT, D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,
		D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT, D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, D3D11_FILTER_MIN_MAG_MIP_LINEAR
	},
	{
		D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT, D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT, D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR,
		D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT, D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT, D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR
	},
};


D3D11_FILTER DX11Sampler::FilterFromDescription(const SamplerDescription& description)
{
	if (description.anisotropicEnabled)
	{
		return gAnisotropicFilters[description.samplingMode];
	}
	else
	{
		size_t index = (description.minFilter << 2) | (description.magFilter << 1) | description.mipFilter;
		return gRegularFilters[description.samplingMode][index];
	}
}