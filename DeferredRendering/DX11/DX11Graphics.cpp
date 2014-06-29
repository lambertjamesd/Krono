#include "stdafx.h"
#include "DX11Graphics.h"
#include "HResultException.h"
#include "DX11WindowRenderBuffer.h"
#include "DX11VertexBuffer.h"
#include "DX11Shader.h"
#include "DX11ShaderProgram.h"
#include <vector>

#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")

using namespace std;

DX11Graphics::DX11Graphics(void)
{
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	// TODO add error checking
	HRESULT result = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		&mDevice,
		&mFeatureLevel,
		&mDeviceContext);

	if (FAILED(result))
	{
		throw HResultException(result, "Could not initialize D3D11 Device");
	}

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&mDXGIFactory);
	
	if (FAILED(result))
	{
		throw HResultException(result, "Could not create IDXGIFactory");
	}
}


DX11Graphics::~DX11Graphics(void)
{
	mDXGIFactory->Release();
	mDevice->Release();
	mDeviceContext->Release();
}

Auto<VertexBuffer> DX11Graphics::CreateVertexBuffer(const InputLayout& inputLayout)
{
	return Auto<VertexBuffer>(new DX11VertexBuffer(mDevice, inputLayout));
}

Auto<VertexShader> DX11Graphics::CreateVertexShader(const std::string& source)
{
	return Auto<VertexShader>(new DX11VertexShader(mDevice, source));
}

Auto<FragmentShader> DX11Graphics::CreateFragmentShader(const std::string& source)
{
	return Auto<FragmentShader>(new DX11FragmentShader(mDevice, source));
}

Auto<ShaderProgram> DX11Graphics::CreateShaderProgram(const Auto<VertexShader>& vertexShader, const Auto<FragmentShader>& fragmentShader)
{
	return Auto<ShaderProgram>(new DX11ShaderProgram(mDevice, std::dynamic_pointer_cast<DX11VertexShader>(vertexShader), std::dynamic_pointer_cast<DX11FragmentShader>(fragmentShader)));
}

Auto<WindowRenderTarget> DX11Graphics::CreateWindowRenderTarget(Window& targetWindow)
{
	return Auto<WindowRenderTarget>(new DX11WindowRenderBuffer(*this, targetWindow));
}

void DX11Graphics::SetViewport(const Rectf& viewport, const Rangef& depthRange)
{
	D3D11_VIEWPORT dxViewport;
	ZeroMemory(&dxViewport, sizeof(D3D11_VIEWPORT));
	dxViewport.TopLeftX = viewport.topLeft.x;
	dxViewport.TopLeftY = viewport.topLeft.y;
	dxViewport.Width = viewport.size.x;
	dxViewport.Height = viewport.size.y;
	dxViewport.MinDepth = depthRange.start;
	dxViewport.MaxDepth  = depthRange.start + depthRange.length;

	mDeviceContext->RSSetViewports(1, &dxViewport);
}

void DX11Graphics::BindRenderTargets(int count, RenderTarget *const* renderTargets, const DepthBuffer* depthBuffer)
{
	vector<ID3D11RenderTargetView*> dxRenderTargets(count);
	ID3D11DepthStencilView* depthBufferView;

	if (depthBuffer != NULL)
	{
		depthBuffer->GetRenderTargetInternal(&depthBufferView);
	}
	else
	{
		depthBufferView = NULL;
	}

	ID3D11RenderTargetView** dxRenderTargetPointer = &(dxRenderTargets.front());
	for (int i = 0; i < count; ++i, ++dxRenderTargetPointer)
	{
		renderTargets[i]->GetRenderTargetInternal(dxRenderTargetPointer);
	}

	mDeviceContext->OMSetRenderTargets(count, &(dxRenderTargets.front()), depthBufferView);
}

ID3D11Device* DX11Graphics::GetDevice()
{
	return mDevice;
}

ID3D11DeviceContext* DX11Graphics::GetContext()
{
	return mDeviceContext;
}

IDXGIFactory* DX11Graphics::GetDXGIFactory()
{
	return mDXGIFactory;
}

void DX11Graphics::Draw(size_t count, size_t offset)
{
	mDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->Draw(count, offset);
}