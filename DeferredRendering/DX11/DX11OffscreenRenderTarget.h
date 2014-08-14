#pragma once
#include "Interface\RenderTarget.h"
#include "Interface\DataFormat.h"
#include "DX11Texture2D.h"
#include "DX11RenderTarget.h"

class DX11OffscreenRenderTarget : public RenderTarget, public DX11RenderTarget
{
public:
	DX11OffscreenRenderTarget(ID3D11Device* device, Vector2i size, DataFormat format);
	~DX11OffscreenRenderTarget(void);
	
	virtual Auto<Texture2D> GetTexture() const;
	void Clear(const Colorf& color);
	
	virtual ID3D11RenderTargetView* GetTargetView();
private:
	ID3D11Device *mDevice;
	ID3D11RenderTargetView *mRenderTargetView;
	Auto<DX11Texture2D> mTexture;
};

