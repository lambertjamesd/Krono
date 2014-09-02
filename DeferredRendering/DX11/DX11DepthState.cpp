#include "DX11DepthState.h"
#include "HResultException.h"

namespace krono
{

DX11DepthState::DX11DepthState(ID3D11Device* device, const DepthStateDescription& description) :
	DepthState(description)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	depthStencilDesc.DepthEnable = description.depthEnable;
	depthStencilDesc.DepthWriteMask = description.depthWriteEnabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = gComparisonFunction[description.depthCompare];
	depthStencilDesc.StencilEnable = description.stencilEnable;
	depthStencilDesc.StencilReadMask = description.stencilReadMask;
	depthStencilDesc.StencilWriteMask = description.stencilWriteMask;
	depthStencilDesc.FrontFace = BuildStencilDescription(description.frontFace);
	depthStencilDesc.BackFace = BuildStencilDescription(description.backFace);

	HRESULT result = device->CreateDepthStencilState(&depthStencilDesc, &mDepthStencil);

	if (FAILED(result))
	{
		throw HResultException(result, "Could not create depth state");
	}
}

DX11DepthState::~DX11DepthState(void)
{
}

D3D11_COMPARISON_FUNC DX11DepthState::GetComparisonFunction(CompareFunction::Type compareFunction)
{
	return gComparisonFunction[compareFunction];
}

const ID3D11DepthStencilState* DX11DepthState::GetDepthStencilState() const
{
	return mDepthStencil.Get();
}

D3D11_DEPTH_STENCILOP_DESC DX11DepthState::BuildStencilDescription(const StencilFunction& stencilFunction)
{
	D3D11_DEPTH_STENCILOP_DESC result;
	result.StencilFailOp = gStencilOperations[stencilFunction.stencilFail];
	result.StencilDepthFailOp = gStencilOperations[stencilFunction.stencilDepthFail];
	result.StencilPassOp = gStencilOperations[stencilFunction.stencilPass];
	result.StencilFunc = gComparisonFunction[stencilFunction.stencilFunction];
	return result;
}

D3D11_COMPARISON_FUNC DX11DepthState::gComparisonFunction[CompareFunction::Count] = {
	D3D11_COMPARISON_NEVER,
	D3D11_COMPARISON_LESS,
	D3D11_COMPARISON_EQUAL,
	D3D11_COMPARISON_LESS_EQUAL,
	D3D11_COMPARISON_GREATER,
	D3D11_COMPARISON_NOT_EQUAL,
	D3D11_COMPARISON_GREATER_EQUAL,
	D3D11_COMPARISON_ALWAYS
};

D3D11_STENCIL_OP DX11DepthState::gStencilOperations[StencilOperation::Count] = {
	D3D11_STENCIL_OP_KEEP,
	D3D11_STENCIL_OP_ZERO,
	D3D11_STENCIL_OP_REPLACE,
	D3D11_STENCIL_OP_INCR_SAT,
	D3D11_STENCIL_OP_DECR_SAT,
	D3D11_STENCIL_OP_INVERT,
	D3D11_STENCIL_OP_INCR,
	D3D11_STENCIL_OP_DECR
};

}