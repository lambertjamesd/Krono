
#include "DX11Texture2D.h"
#include "DX11Graphics.h"
#include "HResultException.h"


DX11Texture2D::DX11Texture2D(ID3D11Device* device, const Vector2i& size, DataFormat format) :
	Texture2D(size, format),
	mDevice(device)
{
	D3D11_TEXTURE2D_DESC textureDescription;
	
	ZeroMemory(&textureDescription, sizeof(textureDescription));
	textureDescription.Width = size.x;
	textureDescription.Height = size.y;
	textureDescription.MipLevels = 1;
	textureDescription.ArraySize = 1;
	textureDescription.Format = DX11Graphics::GetDXFormat(format);
	textureDescription.SampleDesc.Count = 1;
	textureDescription.Usage = D3D11_USAGE_DYNAMIC;
	textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureDescription.MiscFlags = 0;

	HRESULT result = mDevice->CreateTexture2D(&textureDescription, NULL, &mTexture);

	if (FAILED(result))
	{
		throw HResultException(result, "Could not create render target texture");
	}
	
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDescription;
	shaderResourceDescription.Format = DX11Graphics::GetDXFormat(format);
	shaderResourceDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceDescription.Texture2D.MostDetailedMip = 0;
	shaderResourceDescription.Texture2D.MipLevels = 1;

	result = mDevice->CreateShaderResourceView(mTexture.Get(), &shaderResourceDescription, &mShaderResourceView);

	if (FAILED(result))
	{
		mTexture = NULL;
		throw HResultException(result, "Could not create shader resource view");
	}
}

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

void DX11Texture2D::LoadMemory(void* source)
{
	ID3D11DeviceContext *context;
	mDevice->GetImmediateContext(&context);
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = context->Map(mTexture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	
	if (FAILED(result))
	{
		throw HResultException(result, "Could not lock constant buffer");
	}

	memcpy(mappedResource.pData, source, mFormat.GetSize() * mSize.x * mSize.y);

	context->Unmap(mTexture.Get(), 0);
}