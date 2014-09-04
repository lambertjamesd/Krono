
#include "DX11RasterizerState.h"
#include"HResultException.h"

namespace krono
{
	
DX11RasterizerState::DX11RasterizerState(ID3D11Device* device, const RasterizerStateDescription& description) :
	RasterizerState(description)
{
	D3D11_RASTERIZER_DESC rasterizerDesc;

	rasterizerDesc.FillMode = gFillMode[description.fillMode];
	rasterizerDesc.CullMode = gCullMode[description.cullMode];
	rasterizerDesc.FrontCounterClockwise = description.frontCCW;
	rasterizerDesc.DepthBias = description.depthBias;
	rasterizerDesc.DepthBiasClamp = description.depthBiasClamp;
	rasterizerDesc.SlopeScaledDepthBias = description.slopeScaledDepthBias;
	rasterizerDesc.DepthClipEnable = description.depthClipEnable;
	rasterizerDesc.ScissorEnable = description.scissorEnable;
	rasterizerDesc.MultisampleEnable = description.multisampleEnable;
	rasterizerDesc.AntialiasedLineEnable = description.antialiasedLineEnable;

	HRESULT result = device->CreateRasterizerState(&rasterizerDesc, &mRasterizerState);
	
	if (FAILED(result))
	{
		throw HResultException(result, "Could not create depth state");
	}
}

DX11RasterizerState::~DX11RasterizerState()
{

}

ID3D11RasterizerState* DX11RasterizerState::GetRasterizerState()
{
	return mRasterizerState.Get();
}

D3D11_FILL_MODE DX11RasterizerState::gFillMode[FillMode::Count] = {
	D3D11_FILL_WIREFRAME,
	D3D11_FILL_SOLID
};

D3D11_CULL_MODE DX11RasterizerState::gCullMode[CullMode::Count] = {
	D3D11_CULL_NONE,
	D3D11_CULL_FRONT,
	D3D11_CULL_BACK
};

}