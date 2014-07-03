#pragma once
#include "..\Interface\Window.h"
#include "..\Interface\RenderTarget.h"

class DX11Graphics;

class DX11WindowRenderTarget : public WindowRenderTarget
{
public:
	DX11WindowRenderTarget(DX11Graphics& graphics, Window& window);
	~DX11WindowRenderTarget(void);
	
	virtual void GetRenderTargetInternal(void* target) const;
	virtual Auto<Texture2D> GetTexture() const;
	virtual void Clear(const Colorf& color);
	
	virtual void Present(void);
private:
	ID3D11DeviceContext *mDeviceContext;
	IDXGISwapChain *mSwapChain;
	ID3D11RenderTargetView *mRenderTargetView;
};

