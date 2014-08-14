
#include "DX11WindowRenderTarget.h"
#include "DX11Graphics.h"
#include "HResultException.h"

DX11WindowRenderTarget::DX11WindowRenderTarget(DX11Graphics& graphics, Window& window) :
	WindowRenderTarget(window.GetSize()),
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


DX11WindowRenderTarget::~DX11WindowRenderTarget(void)
{
}

void DX11WindowRenderTarget::Present(void)
{
	mSwapChain->Present(1, 0);
}

ID3D11RenderTargetView* DX11WindowRenderTarget::GetTargetView()
{
	return mRenderTargetView;
}

Auto<Texture2D> DX11WindowRenderTarget::GetTexture() const
{
	return Auto<Texture2D>(NULL);
}

void DX11WindowRenderTarget::Clear(const Colorf& color)
{
	FLOAT colorArray[] = {color.r, color.g, color.b, color.a};
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, colorArray);
}