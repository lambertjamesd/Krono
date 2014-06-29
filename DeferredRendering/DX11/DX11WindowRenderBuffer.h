#pragma once
#include "..\Interface\Window.h"
#include "..\Interface\RenderTarget.h"

class DX11Graphics;

class DX11WindowRenderBuffer : public WindowRenderTarget
{
public:
	DX11WindowRenderBuffer(DX11Graphics& graphics, Window& window);
	~DX11WindowRenderBuffer(void);
	
	virtual void GetRenderTargetInternal(void* target) const;
	virtual void Clear(const Colorf& color);
	
	virtual void Present(void);
private:
	ID3D11DeviceContext *mDeviceContext;
	IDXGISwapChain *mSwapChain;
	ID3D11RenderTargetView *mRenderTargetView;
};

