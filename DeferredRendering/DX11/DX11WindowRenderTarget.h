#pragma once

#include "DX11Common.h"

#include "Interface/Window.h"
#include "Interface/RenderTarget.h"
#include "DX11RenderTarget.h"

class DX11Graphics;

class DX11WindowRenderTarget : public WindowRenderTarget, public DX11RenderTarget
{
public:
	DX11WindowRenderTarget(DX11Graphics& graphics, Window& window);
	~DX11WindowRenderTarget(void);
	
	virtual Auto<Texture2D> GetTexture() const;
	virtual void Clear(const Colorf& color);
	
	virtual ID3D11RenderTargetView* GetTargetView();

	virtual void Present(void);
private:
	ID3D11DeviceContext *mDeviceContext;
	IDXGISwapChain *mSwapChain;
	ID3D11RenderTargetView *mRenderTargetView;
};

