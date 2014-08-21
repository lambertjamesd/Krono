

#include "DX11Shader.h"
#include "HResultException.h"
#include "Interface/UnsupportedFormatException.h"
#include <vector>
#include "DX11Graphics.h"

using namespace std;

DX11VertexShader::DX11VertexShader(ID3D11Device* device, const std::string& compiledData) :
		mBytecode(compiledData),
		mVertexShader(NULL),
		mDevice(device)
{
	HRESULT result = device->CreateVertexShader(&compiledData.front(), compiledData.length(), NULL, &mVertexShader);
	
	if (FAILED(result))
	{
		throw HResultException(result, "Could not create vertex shader");
	}
}


DX11VertexShader::~DX11VertexShader(void)
{
	if (mVertexShader != NULL)
	{
		mVertexShader->Release();
		mVertexShader = NULL;
	}
}

bool DX11VertexShader::IsValid() const
{
	return mVertexShader != NULL;
}

ID3D11InputLayout* DX11VertexShader::GetInputLayout(const InputLayout& inputLayout)
{
	auto existing = mInputLayouts.find(inputLayout.GetSignature());

	ID3D11InputLayout* result;

	if (existing == mInputLayouts.end())
	{
		result = CreateInputLayout(inputLayout);
		mInputLayouts[inputLayout.GetSignature()] = result;
		result->Release();
	}
	else
	{
		result = existing->second.Get();
	}

	return result;
}

ID3D11InputLayout* DX11VertexShader::CreateInputLayout(const InputLayout& inputLayout) const
{
	vector<D3D11_INPUT_ELEMENT_DESC> inputElements;

	size_t offset = 0;

	for (size_t i = 0; i < inputLayout.GetAttributeCount(); ++i)
	{
		const Attribute& attribute = inputLayout.GetAttribute(i);
		D3D11_INPUT_ELEMENT_DESC inputElement;
		ZeroMemory(&inputElement, sizeof(D3D11_INPUT_ELEMENT_DESC));

		DXGI_FORMAT format = DX11Graphics::GetDXFormat(attribute.GetFormat());

		if (format == DXGI_FORMAT_UNKNOWN)
		{
			throw UnsupportedFormatException("Unsupported vertex attribute format");
		}

		inputElement.SemanticName = attribute.GetName().c_str();
		inputElement.SemanticIndex = attribute.GetIndex();
		inputElement.Format = format;
		inputElement.InputSlot = 0;
		inputElement.AlignedByteOffset = offset;
		inputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputElement.InstanceDataStepRate = 0;

		inputElements.push_back(inputElement);

		offset += attribute.GetFormat().GetSize();

	}

	ID3D11InputLayout *result;
	HRESULT hresult = mDevice->CreateInputLayout(&inputElements.front(), inputElements.size(), &mBytecode.front(), mBytecode.size(), &result);

	if (FAILED(hresult))
	{
		throw HResultException(hresult, "Could not create ID3D11InputLayout");
	}

	return result;
}

void DX11VertexShader::Use()
{
	ID3D11DeviceContext *context;
	mDevice->GetImmediateContext(&context);

	context->VSSetShader(mVertexShader, NULL, 0);
}

DX11PixelShader::DX11PixelShader(ID3D11Device* device, const std::string& compiledData) :
		mPixelShader(NULL),
		mDevice(device)
{
	HRESULT result = device->CreatePixelShader(&compiledData.front(), compiledData.length(), NULL, &mPixelShader);
	
	if (FAILED(result))
	{
		throw HResultException(result, "Could not create vertex shader");
	}
}

DX11PixelShader::~DX11PixelShader(void)
{
	if (mPixelShader != NULL)
	{
		mPixelShader->Release();
		mPixelShader = NULL;
	}
}

bool DX11PixelShader::IsValid() const
{
	return mPixelShader != NULL;
}

void DX11PixelShader::Use()
{
	ID3D11DeviceContext *context;
	mDevice->GetImmediateContext(&context);
	context->PSSetShader(mPixelShader, NULL, 0);
}