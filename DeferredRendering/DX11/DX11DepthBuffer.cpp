
#include "DX11DepthBuffer.h"
#include "HResultException.h"
#include "DX11Graphics.h"

DXGI_FORMAT DX11DepthBuffer::gTextureFormatMapping[DataFormat::TypeCount] =
{
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,

	DXGI_FORMAT_R16_TYPELESS,
	DXGI_FORMAT_R24G8_TYPELESS,
	DXGI_FORMAT_R32_TYPELESS,
	DXGI_FORMAT_R24G8_TYPELESS,
	DXGI_FORMAT_R32G8X24_TYPELESS
};

DXGI_FORMAT DX11DepthBuffer::gDepthViewFormatmapping[DataFormat::TypeCount] =
{
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,

	DXGI_FORMAT_D16_UNORM,
	DXGI_FORMAT_D24_UNORM_S8_UINT,
	DXGI_FORMAT_D32_FLOAT,
	DXGI_FORMAT_D24_UNORM_S8_UINT,
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT
};

DX11DepthBuffer::DX11DepthBuffer(ID3D11Device *device, Vector2i size, DataFormat::Type format) :
	DepthBuffer(size, format),
	mDevice(device)
{
	DataFormat dataFormat(format, 1);

	D3D11_TEXTURE2D_DESC textureDescription;
	
	ZeroMemory(&textureDescription, sizeof(textureDescription));
	textureDescription.Width = size.x;
	textureDescription.Height = size.y;
	textureDescription.MipLevels = 1;
	textureDescription.ArraySize = 1;
	textureDescription.Format = gTextureFormatMapping[format];
	textureDescription.SampleDesc.Count = 1;
	textureDescription.Usage = D3D11_USAGE_DEFAULT;
	textureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDescription.CPUAccessFlags = 0;
	textureDescription.MiscFlags = 0;

	ID3D11Texture2D *texture;

	HRESULT result = mDevice->CreateTexture2D(&textureDescription, NULL, &texture);

	if (FAILED(result))
	{
		throw HResultException(result, "Could not create render target texture");
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthView;
	depthView.Format = gDepthViewFormatmapping[format];
	depthView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthView.Flags = 0;
	depthView.Texture2D.MipSlice = 0;

	result = mDevice->CreateDepthStencilView(texture, &depthView, &mDepthTargetView);

	if (FAILED(result))
	{
		texture->Release();
		throw HResultException(result, "Could not create render target view");
	}

	mTexture.reset(new DX11Texture2D(mDevice, texture, size, dataFormat));
}


DX11DepthBuffer::~DX11DepthBuffer(void)
{

}

	
ID3D11DepthStencilView* DX11DepthBuffer::GetDepthView()
{
	return mDepthTargetView;
}

Auto<Texture2D> DX11DepthBuffer::GetTexture() const
{
	return mTexture;
}

void DX11DepthBuffer::Clear(float value)
{
	ID3D11DeviceContext *deviceContext;
	mDevice->GetImmediateContext(&deviceContext);
	deviceContext->ClearDepthStencilView(mDepthTargetView, D3D11_CLEAR_DEPTH, value, 0);
}