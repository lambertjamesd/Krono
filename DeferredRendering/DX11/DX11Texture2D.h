#pragma once

#include "Interface/Texture2D.h"
#include "DX11SmartPtr.h"
#include "IDX11Resource.h"

class DX11Texture2D : public Texture2D, public IDX11Resource
{
public:
	DX11Texture2D(ID3D11Device* device, const Vector2i& size, DataFormat format);
	DX11Texture2D(ID3D11Device* device, ID3D11Texture2D *texture, const Vector2i& size, DataFormat format);
	~DX11Texture2D(void);

	ID3D11Texture2D *GetTexture();
	virtual ID3D11ShaderResourceView *GetResource();
	
	virtual void LoadMemory(void* source);
	
	virtual void GenerateMipmaps();
private:
	ID3D11Device *mDevice;
	DX11SmartPtr<ID3D11Texture2D> mTexture;
	DX11SmartPtr<ID3D11ShaderResourceView> mShaderResourceView;
};

