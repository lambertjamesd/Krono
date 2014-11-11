
#include "DX11OffscreenRenderTarget.h"
#include "DX11Graphics.h"
#include "HResultException.h"
#include "DX11Texture2D.h"

namespace krono
{

DX11OffscreenRenderTarget::DX11OffscreenRenderTarget(ID3D11Device* device, Vector2i size, DataFormat format) :
	RenderTarget(size, format),
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
	textureDescription.Usage = D3D11_USAGE_DEFAULT;
	textureDescription.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDescription.CPUAccessFlags = 0;
	textureDescription.MiscFlags = 0;

	ID3D11Texture2D *texture;

	HRESULT result = mDevice->CreateTexture2D(&textureDescription, NULL, &texture);

	if (FAILED(result))
	{
		throw HResultException(result, "Could not create render target texture");
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetView;
	renderTargetView.Format = textureDescription.Format;
	renderTargetView.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetView.Texture2D.MipSlice = 0;

	result = mDevice->CreateRenderTargetView(texture, &renderTargetView, &mRenderTargetView);

	if (FAILED(result))
	{
		texture->Release();
		throw HResultException(result, "Could not create render target view");
	}

	mTexture.reset(new DX11Texture2D(mDevice, texture, size, format));
}


DX11OffscreenRenderTarget::~DX11OffscreenRenderTarget(void)
{
	mRenderTargetView->Release();
}

ID3D11RenderTargetView* DX11OffscreenRenderTarget::GetTargetView()
{
	return mRenderTargetView;
}

Auto<Texture2D> DX11OffscreenRenderTarget::GetTexture() const
{
	return mTexture;
}

void DX11OffscreenRenderTarget::Clear(const Colorf& color)
{
	FLOAT colorArray[] = {color.r, color.g, color.b, color.a};
	ID3D11DeviceContext *deviceContext;
	mDevice->GetImmediateContext(&deviceContext);
	deviceContext->ClearRenderTargetView(mRenderTargetView, colorArray);
}

}
