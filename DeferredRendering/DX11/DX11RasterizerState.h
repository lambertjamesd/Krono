#pragma once

#include "Interface/RasterizerState.h"
#include "DX11Common.h"
#include "DX11SmartPtr.h"

namespace krono
{

class DX11RasterizerState : public RasterizerState
{
public:
	DX11RasterizerState(ID3D11Device* device, const RasterizerStateDescription& description);
	~DX11RasterizerState(void);

	ID3D11RasterizerState* GetRasterizerState();
private:
	DX11SmartPtr<ID3D11RasterizerState> mRasterizerState;

	static D3D11_FILL_MODE gFillMode[FillMode::Count];
	static D3D11_CULL_MODE gCullMode[CullMode::Count];
};

}