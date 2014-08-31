#include "DX11BlendState.h"
#include "HResultException.h"

namespace krono
{

DX11BlendState::DX11BlendState(ID3D11Device* device, const BlendStateDescription& description) :
	BlendState(description)
{
	D3D11_BLEND_DESC blendDesc;

	blendDesc.AlphaToCoverageEnable = description.alphaToCoverage;
	blendDesc.IndependentBlendEnable = description.independentBlend;

	for (size_t i = 0; i < MaxRenderTargetCount; ++i)
	{
		ConvertRenderTarget(blendDesc.RenderTarget[i], description.renderTargets[i]);
	}

	HRESULT result = device->CreateBlendState(&blendDesc, &mBlendState);
		
	if (FAILED(result))
	{
		throw HResultException(result, "Could not create blend state");
	}
}


DX11BlendState::~DX11BlendState(void)
{

}

ID3D11BlendState* DX11BlendState::GetBlendState()
{
	return mBlendState.Get();
}

const D3D11_BLEND DX11BlendState::gBlendMapping[Blend::Count] = {
	D3D11_BLEND_ZERO,
	D3D11_BLEND_ONE,
	D3D11_BLEND_SRC_COLOR,
	D3D11_BLEND_INV_SRC_COLOR,
	D3D11_BLEND_SRC_ALPHA,
	D3D11_BLEND_INV_SRC_ALPHA,
	D3D11_BLEND_DEST_ALPHA,
	D3D11_BLEND_INV_DEST_ALPHA,
	D3D11_BLEND_DEST_COLOR,
	D3D11_BLEND_INV_DEST_COLOR,
	D3D11_BLEND_SRC_ALPHA_SAT,
	D3D11_BLEND_BLEND_FACTOR,
	D3D11_BLEND_INV_BLEND_FACTOR,
	D3D11_BLEND_SRC1_COLOR,
	D3D11_BLEND_INV_SRC1_COLOR,
	D3D11_BLEND_SRC1_ALPHA,
	D3D11_BLEND_INV_SRC1_ALPHA
};

const D3D11_BLEND_OP DX11BlendState::gOperatorMapping[BlendOperator::Count] = {
	D3D11_BLEND_OP_ADD,
	D3D11_BLEND_OP_SUBTRACT,
	D3D11_BLEND_OP_REV_SUBTRACT,
	D3D11_BLEND_OP_MIN,
	D3D11_BLEND_OP_MAX
};

void DX11BlendState::ConvertRenderTarget(D3D11_RENDER_TARGET_BLEND_DESC& target, const RenderTargetBlend& source)
{
	target.BlendEnable = source.blendEnabled;
	target.SrcBlend = gBlendMapping[source.srcBlend];
	target.DestBlend = gBlendMapping[source.destBlend];
	target.BlendOp = gOperatorMapping[source.blendOperation];

	target.SrcBlendAlpha = gBlendMapping[source.srcBlendAlpha];
	target.DestBlendAlpha = gBlendMapping[source.destBlendAlpha];
	target.BlendOpAlpha = gOperatorMapping[source.blendOperationAlpha];

	target.RenderTargetWriteMask = 
		((source.blendMask & BlendMask::Red) ? D3D11_COLOR_WRITE_ENABLE_RED : 0) |
		((source.blendMask & BlendMask::Green) ? D3D11_COLOR_WRITE_ENABLE_GREEN : 0) |
		((source.blendMask & BlendMask::Blue) ? D3D11_COLOR_WRITE_ENABLE_BLUE : 0) |
		((source.blendMask & BlendMask::Alpha) ? D3D11_COLOR_WRITE_ENABLE_ALPHA : 0);
}

}