#pragma once

#include "Interface/BlendState.h"
#include "DX11Common.h"
#include "DX11SmartPtr.h"

namespace krono
{

class DX11BlendState : public BlendState
{
public:
	DX11BlendState(ID3D11Device* device, const BlendStateDescription& description);
	~DX11BlendState(void);

	ID3D11BlendState* GetBlendState();
private:
	DX11SmartPtr<ID3D11BlendState> mBlendState;

	static const D3D11_BLEND gBlendMapping[Blend::Count];
	static const D3D11_BLEND_OP gOperatorMapping[BlendOperator::Count];

	static void ConvertRenderTarget(D3D11_RENDER_TARGET_BLEND_DESC& target, const RenderTargetBlend& source);
};

}