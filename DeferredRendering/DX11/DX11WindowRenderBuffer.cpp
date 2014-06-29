#include "stdafx.h"
#include "DX11WindowRenderBuffer.h"
#include "DX11Graphics.h"
#include "HResultException.h"

DX11WindowRenderBuffer::DX11WindowRenderBuffer(DX11Graphics& graphics, Window& window) :
	mDeviceContext(graphics.GetContext())
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc.Width = window.GetSize().x;
	swapChainDesc.BufferDesc.Height = window.GetSize().y;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = window.GetWindowHandle();

	swapChainDesc.Windowed = true;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;
		
	HRESULT result = graphics.GetDXGIFactory()->CreateSwapChain(graphics.GetDevice(), &swapChainDesc, &mSwapChain);
	
	if (FAILED(result))
	{
		throw HResultException(result, "Could not initialize swap buffer");
	}
	
	ID3D11Texture2D *backBuffer;
	result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	
	if (FAILED(result))
	{
		throw HResultException(result, "Could not retrieve back buffer");
	}

	result = graphics.GetDevice()->CreateRenderTargetView(backBuffer, NULL, &mRenderTargetView);

	backBuffer->Release();
	
	if (FAILED(result))
	{
		throw HResultException(result, "Could not create render target view");
	}
}


DX11WindowRenderBuffer::~DX11WindowRenderBuffer(void)
{
}

void DX11WindowRenderBuffer::Present(void)
{
	mSwapChain->Present(1, 0);
}

void DX11WindowRenderBuffer::GetRenderTargetInternal(void* target) const
{
	*((ID3D11RenderTargetView**)target) = mRenderTargetView;
}

void DX11WindowRenderBuffer::Clear(const Colorf& color)
{
	FLOAT colorArray[] = {color.r, color.g, color.b, color.a};
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, colorArray);
}