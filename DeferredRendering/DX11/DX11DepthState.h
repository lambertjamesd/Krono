#pragma once

#include "Interface/DepthState.h"
#include "DX11Common.h"
#include "DX11SmartPtr.h"

namespace krono
{

class DX11DepthState : public DepthState
{
public:
	DX11DepthState(ID3D11Device* device, const DepthStateDescription& description);
	~DX11DepthState(void);

	static D3D11_COMPARISON_FUNC GetComparisonFunction(CompareFunction::Type compareFunction);
	ID3D11DepthStencilState* GetDepthStencilState();

private:
	static D3D11_DEPTH_STENCILOP_DESC BuildStencilDescription(const StencilFunction& stencilFunction);

	static D3D11_COMPARISON_FUNC gComparisonFunction[CompareFunction::Count];
	static D3D11_STENCIL_OP gStencilOperations[StencilOperation::Count];

	DX11SmartPtr<ID3D11DepthStencilState> mDepthStencil;
};

}