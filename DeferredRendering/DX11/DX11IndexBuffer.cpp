
#include "DX11IndexBuffer.h"
#include "HResultException.h"


DX11IndexBuffer::DX11IndexBuffer(ID3D11Device *device, IndexBuffer::Format format) :
	IndexBuffer(format),
	mDevice(device),
	mCurrentIndexCount(-1)
{

}


DX11IndexBuffer::~DX11IndexBuffer(void)
{
	CleanUp();
}


DataIterator DX11IndexBuffer::Lock(size_t indexCount)
{
	if (indexCount != mCurrentIndexCount)
	{
		RebuildBuffer(indexCount);
	}

	ID3D11DeviceContext *context;
	mDevice->GetImmediateContext(&context);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = context->Map(mBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	
	if (FAILED(result))
	{
		throw HResultException(result, "Could not lock vertex buffer");
	}

	return DataIterator(mappedResource.pData, (char*)mappedResource.pData + mCurrentIndexCount * GetFormatSize(GetFormat()));
}

void DX11IndexBuffer::Unlock()
{
	ID3D11DeviceContext *context;
	mDevice->GetImmediateContext(&context);

	context->Unmap(mBuffer.Get(), 0);
}

size_t DX11IndexBuffer::GetIndexCount() const
{
	return mCurrentIndexCount;
}

DXGI_FORMAT DX11IndexBuffer::gFormatMapping[IndexBuffer::FormatCount] = {
	DXGI_FORMAT_R16_UINT,
	DXGI_FORMAT_R32_UINT
};

DXGI_FORMAT DX11IndexBuffer::GetDXFormat() const
{
	return gFormatMapping[GetFormat()];
}

ID3D11Buffer* DX11IndexBuffer::GetBuffer()
{
	return mBuffer.Get();
}

void DX11IndexBuffer::CleanUp()
{
	mBuffer = NULL;
}

void DX11IndexBuffer::RebuildBuffer(size_t indexCount)
{
	CleanUp();
	
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = GetStrideSize() * indexCount;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ID3D11Buffer *newBuffer;
	HRESULT result = mDevice->CreateBuffer(&bufferDesc, NULL, &newBuffer);

	if (FAILED(result))
	{
		throw HResultException(result, "Could not create vertex buffer");
	}
	
	mBuffer = newBuffer;
	mCurrentIndexCount = indexCount;
}