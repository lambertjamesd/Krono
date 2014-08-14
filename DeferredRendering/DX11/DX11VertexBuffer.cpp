
#include "DX11VertexBuffer.h"
#include "HResultException.h"
#include "Core\Hash.h"

DX11VertexBuffer::DX11VertexBuffer(ID3D11Device *device, const InputLayout& inputLayout) : 
	VertexBuffer(inputLayout),
	mBuffer(NULL),
	mDevice(device),
	mCurrentVertexCount(0)
{

}


DX11VertexBuffer::~DX11VertexBuffer(void)
{
	if (mBuffer != NULL)
	{
		mBuffer->Release();
	}
}

void DX11VertexBuffer::CleanUp()
{
	if (mBuffer != NULL)
	{
		mBuffer->Release();
	}
}

void DX11VertexBuffer::RebuildBuffer(size_t vertexCount)
{
	CleanUp();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = mInputLayout.GetStride() * vertexCount;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.StructureByteStride = mInputLayout.GetStride();
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ID3D11Buffer *newBuffer;
	HRESULT result = mDevice->CreateBuffer(&bufferDesc, NULL, &newBuffer);

	if (FAILED(result))
	{
		throw HResultException(result, "Could not create vertex buffer");
	}
	
	mBuffer = newBuffer;
	mCurrentVertexCount = vertexCount;
}

DataIterator DX11VertexBuffer::Lock(size_t vertexCount)
{
	size_t bufferSize = vertexCount * mInputLayout.GetStride();

	if (mBuffer == NULL || mCurrentVertexCount != vertexCount)
	{
		RebuildBuffer(vertexCount);
	}

	ID3D11DeviceContext *context;
	mDevice->GetImmediateContext(&context);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = context->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	
	if (FAILED(result))
	{
		throw HResultException(result, "Could not lock vertex buffer");
	}

	return DataIterator(mappedResource.pData, (char*)mappedResource.pData + bufferSize);
}

void DX11VertexBuffer::Unlock()
{
	ID3D11DeviceContext *context;
	mDevice->GetImmediateContext(&context);

	context->Unmap(mBuffer, 0);
}

size_t DX11VertexBuffer::GetVertexCount() const
{
	return mCurrentVertexCount;
}

void DX11VertexBuffer::Use()
{
	ID3D11DeviceContext *context;
	mDevice->GetImmediateContext(&context);

	UINT stride = mInputLayout.GetStride();
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mBuffer, &stride, &offset);
}