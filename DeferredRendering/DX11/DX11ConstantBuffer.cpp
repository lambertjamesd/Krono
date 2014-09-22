
#include "DX11ConstantBuffer.h"
#include "HResultException.h"

namespace krono
{

DX11ConstantBuffer::DX11ConstantBuffer(ID3D11Device* device, const ConstantBufferLayout& layout) :
	ConstantBuffer(layout),
	mBuffer(NULL),
	mDevice(device),
	mCurrentSize(0)
{

}

DX11ConstantBuffer::~DX11ConstantBuffer(void)
{
	CleanUp();
}

void DX11ConstantBuffer::CleanUp()
{
	if (mBuffer != NULL)
	{
		mBuffer->Release();
	}
}

void DX11ConstantBuffer::Set(const void* data, size_t size)
{
	size_t alignedSize = AlignSize(size);

	if (alignedSize != mCurrentSize)
	{
		Resize(alignedSize);
	}
	
	ID3D11DeviceContext *context;
	mDevice->GetImmediateContext(&context);
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = context->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	
	if (FAILED(result))
	{
		throw HResultException(result, "Could not lock constant buffer");
	}

	memcpy(mappedResource.pData, data, size);

	context->Unmap(mBuffer, 0);
}

ID3D11Buffer* DX11ConstantBuffer::GetBuffer()
{
	return mBuffer;
}

void DX11ConstantBuffer::Resize(size_t newSize)
{
	CleanUp();
	
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = newSize;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.StructureByteStride = newSize;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ID3D11Buffer *newBuffer;
	HRESULT result = mDevice->CreateBuffer(&bufferDesc, NULL, &newBuffer);

	if (FAILED(result))
	{
		throw HResultException(result, "Could not create constant buffer");
	}
	
	mBuffer = newBuffer;
	mCurrentSize = newSize;
}

// buffer size must be a multiple of 16
size_t DX11ConstantBuffer::AlignSize(size_t size)
{
	size_t blockSize = (size + AlignmentSize - 1) / AlignmentSize;
	return blockSize * AlignmentSize;
}

}
