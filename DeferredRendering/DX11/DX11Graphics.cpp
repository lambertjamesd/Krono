

#include "DX11Graphics.h"
#include "HResultException.h"
#include "DX11WindowRenderTarget.h"
#include "DX11OffscreenRenderTarget.h"
#include "DX11DepthBuffer.h"
#include "DX11IndexBuffer.h"
#include "DX11VertexBuffer.h"
#include "DX11Shader.h"
#include "DX11ConstantBuffer.h"
#include "DX11Texture2D.h"
#include "DX11Sampler.h"
#include <vector>

#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")

using namespace std;

namespace krono
{

DX11Graphics::DX11Graphics(void) :
	mNeedNewInputMapping(false),
	mHasIndexBuffer(false)
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

Auto<IndexBuffer> DX11Graphics::CreateIndexBuffer(IndexBuffer::Format format)
{
	return Auto<IndexBuffer>(new DX11IndexBuffer(mDevice, format));
}

Auto<VertexBuffer> DX11Graphics::CreateVertexBuffer(const InputLayout& inputLayout)
{
	return Auto<VertexBuffer>(new DX11VertexBuffer(mDevice, inputLayout));
}

Auto<ConstantBuffer> DX11Graphics::CreateConstantBuffer(const ConstantBufferLayout& layout)
{
	return Auto<ConstantBuffer>(new DX11ConstantBuffer(mDevice, layout));
}

Auto<VertexShader> DX11Graphics::CreateVertexShader(const std::string& source)
{
	return Auto<VertexShader>(new DX11VertexShader(mDevice, source));
}

Auto<PixelShader> DX11Graphics::CreatePixelShader(const std::string& source)
{
	return Auto<PixelShader>(new DX11PixelShader(mDevice, source));
}

Auto<WindowRenderTarget> DX11Graphics::CreateWindowRenderTarget(Window& targetWindow)
{
	return Auto<WindowRenderTarget>(new DX11WindowRenderTarget(*this, targetWindow));
}

Auto<RenderTarget> DX11Graphics::CreateOffscreenRenderTarget(Vector2i size, DataFormat format)
{
	return Auto<RenderTarget>(new DX11OffscreenRenderTarget(mDevice, size, format));
}

Auto<DepthBuffer> DX11Graphics::CreateDepthBuffer(Vector2i size, DataFormat::Type format)
{
	return Auto<DepthBuffer>(new DX11DepthBuffer(mDevice, size, format));
}

Auto<Texture2D> DX11Graphics::CreateTexture2D(Vector2i size, DataFormat format)
{
	return Auto<Texture2D>(new DX11Texture2D(mDevice, size, format));
}

Auto<Sampler> DX11Graphics::CreateSampler(const SamplerDescription& description)
{
	return Auto<Sampler>(new DX11Sampler(mDevice, description));
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
	dxViewport.MaxDepth  = depthRange.end;

	mDeviceContext->RSSetViewports(1, &dxViewport);
}

void DX11Graphics::Draw(size_t count, size_t offset)
{
	UpdatePendingChanges();
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->Draw(count, offset);
}

void DX11Graphics::DrawIndexed(size_t count, size_t offset)
{
	UpdatePendingChanges();
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->DrawIndexed(count, offset, 0);
}

void DX11Graphics::SetRenderTargets(const std::vector<Auto<RenderTarget> > &renderTargets, const Auto<DepthBuffer> &depthBuffer)
{
	vector<ID3D11RenderTargetView*> dxRenderTargets(renderTargets.size());
	ID3D11DepthStencilView* depthBufferView;

	if (depthBuffer != NULL)
	{
		depthBufferView = dynamic_cast<DX11DepthBuffer*>(depthBuffer.get())->GetDepthView();
	}
	else
	{
		depthBufferView = NULL;
	}

	ID3D11RenderTargetView** dxRenderTargetPointer = &(dxRenderTargets.front());
	for (size_t i = 0; i < renderTargets.size(); ++i, ++dxRenderTargetPointer)
	{
		DX11RenderTarget *dxRenderTarget = dynamic_cast<DX11RenderTarget*>(renderTargets[i].get());
		dxRenderTargetPointer[i] = dxRenderTarget->GetTargetView();
	}

	mDeviceContext->OMSetRenderTargets(renderTargets.size(), &(dxRenderTargets.front()), depthBufferView);
}

void DX11Graphics::SetTexture(const Auto<Texture>& value, size_t slot, ShaderStage::Type stage)
{
	IDX11Resource* resource = dynamic_cast<IDX11Resource*>(value.get());

	if (resource != NULL)
	{
		ID3D11ShaderResourceView *resourceView = resource->GetResource();

		switch (stage)
		{
		case ShaderStage::PixelShader:
			mDeviceContext->PSSetShaderResources(slot, 1, &resourceView);
			break;
		case ShaderStage::HullShader:
			mDeviceContext->HSSetShaderResources(slot, 1, &resourceView);
			break;
		case ShaderStage::DomainShader:
			mDeviceContext->DSSetShaderResources(slot, 1, &resourceView);
			break;
		case ShaderStage::GeometryShader:
			mDeviceContext->GSSetShaderResources(slot, 1, &resourceView);
			break;
		case ShaderStage::VertexShader:
			mDeviceContext->VSSetShaderResources(slot, 1, &resourceView);
			break;
		case ShaderStage::ComputeShader:
			mDeviceContext->CSSetShaderResources(slot, 1, &resourceView);
			break;
		}
	}
}

void DX11Graphics::SetSampler(const Auto<Sampler>& value, size_t slot, ShaderStage::Type stage)
{
	DX11Sampler *sampler = dynamic_cast<DX11Sampler*>(value.get());

	if (sampler != NULL)
	{
		ID3D11SamplerState *samplerState = sampler->GetSamplerState();

		switch (stage)
		{
		case ShaderStage::PixelShader:
			mDeviceContext->PSSetSamplers(slot, 1, &samplerState);
			break;
		case ShaderStage::HullShader:
			mDeviceContext->HSSetSamplers(slot, 1, &samplerState);
			break;
		case ShaderStage::DomainShader:
			mDeviceContext->DSSetSamplers(slot, 1, &samplerState);
			break;
		case ShaderStage::GeometryShader:
			mDeviceContext->GSSetSamplers(slot, 1, &samplerState);
			break;
		case ShaderStage::VertexShader:
			mDeviceContext->VSSetSamplers(slot, 1, &samplerState);
			break;
		case ShaderStage::ComputeShader:
			mDeviceContext->CSSetSamplers(slot, 1, &samplerState);
			break;
		}
	}
}

void DX11Graphics::SetVertexShader(const Auto<VertexShader> &vertexShader)
{
	mNeedNewInputMapping = true;
	mCurrentVertexShader = std::dynamic_pointer_cast<DX11VertexShader>(vertexShader);
	mCurrentVertexShader->Use();
}

void DX11Graphics::SetPixelShader(const Auto<PixelShader> &fragmentShader)
{
	std::dynamic_pointer_cast<DX11PixelShader>(fragmentShader)->Use();
}

void DX11Graphics::SetIndexBuffer(const Auto<IndexBuffer> &indexBuffer)
{
	if (indexBuffer == NULL)
	{
		mDeviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R16_UINT, 0);
		mHasIndexBuffer = false;
	}
	else
	{
		DX11IndexBuffer *dxIndexBuffer = dynamic_cast<DX11IndexBuffer*>(indexBuffer.get());
		mDeviceContext->IASetIndexBuffer(dxIndexBuffer->GetBuffer(), dxIndexBuffer->GetDXFormat(), 0);
		mHasIndexBuffer = true;
	}
}

void DX11Graphics::SetVertexBuffer(const Auto<VertexBuffer> &vertexBuffer)
{
	mNeedNewInputMapping = mNeedNewInputMapping || 
		mCurrentVertexBuffer == NULL ||
		mCurrentVertexBuffer->GetInputLayout().GetSignature() != vertexBuffer->GetInputLayout().GetSignature();

	mCurrentVertexBuffer = std::dynamic_pointer_cast<DX11VertexBuffer>(vertexBuffer);
	mCurrentVertexBuffer->Use();
}

void DX11Graphics::SetConstantBuffer(const Auto<ConstantBuffer>& constantBuffer, size_t slot, ShaderStage::Type stage)
{
	DX11ConstantBuffer *dxContantBuffer = dynamic_cast<DX11ConstantBuffer*>(constantBuffer.get());

	if (dxContantBuffer != NULL)
	{
		ID3D11Buffer *dxBuffer = dxContantBuffer->GetBuffer();

		switch (stage)
		{
		case ShaderStage::PixelShader:
			mDeviceContext->PSSetConstantBuffers(slot, 1, &dxBuffer);
			break;
		case ShaderStage::HullShader:
			mDeviceContext->HSSetConstantBuffers(slot, 1, &dxBuffer);
			break;
		case ShaderStage::DomainShader:
			mDeviceContext->DSSetConstantBuffers(slot, 1, &dxBuffer);
			break;
		case ShaderStage::GeometryShader:
			mDeviceContext->GSSetConstantBuffers(slot, 1, &dxBuffer);
			break;
		case ShaderStage::VertexShader:
			mDeviceContext->VSSetConstantBuffers(slot, 1, &dxBuffer);
			break;
		case ShaderStage::ComputeShader:
			mDeviceContext->CSSetConstantBuffers(slot, 1, &dxBuffer);
			break;
		}
	}
}

bool DX11Graphics::FlipImageOriginY() const
{
	return false;
}

Graphics::ShaderLanguage DX11Graphics::ExpectedShaderLanguage() const
{
	return Graphics::HLSL_5;
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

DXGI_FORMAT DX11Graphics::gFormatMapping[DataFormat::TypeCount][4] =
{
	{DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_R32G32_FLOAT, DXGI_FORMAT_R32G32B32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT},
	{DXGI_FORMAT_R8_UNORM, DXGI_FORMAT_R8G8_UNORM, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_R8G8B8A8_UNORM},
	{DXGI_FORMAT_R16_UNORM, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN},
	{DXGI_FORMAT_R24_UNORM_X8_TYPELESS, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN},
	{DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN},
	{DXGI_FORMAT_R24_UNORM_X8_TYPELESS, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN},
	{DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN},
};

DXGI_FORMAT DX11Graphics::GetDXFormat(const DataFormat& dataFormat)
{
	return gFormatMapping[dataFormat.type][dataFormat.count - 1];
}

void DX11Graphics::UpdatePendingChanges()
{
	if (mNeedNewInputMapping && mCurrentVertexShader != NULL && mCurrentVertexBuffer != NULL)
	{
		mDeviceContext->IASetInputLayout(mCurrentVertexShader->GetInputLayout(mCurrentVertexBuffer->GetInputLayout()));
		mNeedNewInputMapping = false;
	}
}

}
