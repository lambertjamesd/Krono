#include "stdafx.h"
#include "DX11Texture2D.h"
#include "DX11Graphics.h"
#include "HResultException.h"

DX11Texture2D::DX11Texture2D(ID3D11Device* device, ID3D11Texture2D *texture, const Vector2i& size, DataFormat format) :
	Texture2D(size, format),
	mDevice(device),
	mTexture(texture)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDescription;
	shaderResourceDescription.Format = DX11Graphics::GetDXFormat(format);
	shaderResourceDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceDescription.Texture2D.MostDetailedMip = 0;
	shaderResourceDescription.Texture2D.MipLevels = 1;

	HRESULT result = mDevice->CreateShaderResourceView(mTexture.Get(), &shaderResourceDescription, &mShaderResourceView);

	if (FAILED(result))
	{
		mTexture = NULL;
		throw HResultException(result, "Could not create shader resource view");
	}
}

DX11Texture2D::~DX11Texture2D(void)
{

}

ID3D11Texture2D *DX11Texture2D::GetTexture()
{
	return mTexture.Get();
}

ID3D11ShaderResourceView *DX11Texture2D::GetResource()
{
	return mShaderResourceView.Get();
}