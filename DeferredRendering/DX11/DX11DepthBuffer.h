#pragma once

#include "Interface/DepthBuffer.h"
#include "DX11Texture2D.h"

namespace krono
{

class DX11DepthBuffer : public DepthBuffer
{
public:
	DX11DepthBuffer(ID3D11Device *device, Vector2i size, DataFormat::Type format);
	~DX11DepthBuffer(void);
	
	ID3D11DepthStencilView* GetDepthView();

	virtual Auto<Texture2D> GetTexture() const;
	virtual void Clear(float value);
private:
	ID3D11Device *mDevice;
	ID3D11DepthStencilView *mDepthTargetView;
	Auto<DX11Texture2D> mTexture;

	static DXGI_FORMAT gTextureFormatMapping[DataFormat::TypeCount];
	static DXGI_FORMAT gDepthViewFormatmapping[DataFormat::TypeCount];
};

}
