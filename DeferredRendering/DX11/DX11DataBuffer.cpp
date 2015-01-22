#include "DX11DataBuffer.h"
#include "HResultException.h"

namespace krono
{

DX11DataBuffer::DX11DataBuffer(ID3D11Device *device, BufferType type, BufferAccess::Type bufferAccess) :
	DataBuffer(type, bufferAccess),
	mBuffer(NULL),
	mDevice(device)
{

}


DX11DataBuffer::~DX11DataBuffer(void)
{
	mBuffer = NULL;
}

DataIterator DX11DataBuffer::Map(size_t size, BufferMapType::Type mappingType)
{
	if (size != mCurrentSize)
	{
		RebuildBuffer(size, NULL);
	}

	ID3D11DeviceContext *context;
	mDevice->GetImmediateContext(&context);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = context->Map(mBuffer.Get(), 0, gMapMapping[mappingType], 0, &mappedResource);
	
	if (FAILED(result))
	{
		throw HResultException(result, "Could not lock data buffer");
	}

	return DataIterator(mappedResource.pData, (char*)mappedResource.pData + mCurrentSize);
}

void DX11DataBuffer::Unmap()
{
	ID3D11DeviceContext *context;
	mDevice->GetImmediateContext(&context);

	context->Unmap(mBuffer.Get(), 0);
}

void DX11DataBuffer::Set(const void* data, size_t size)
{
	if ((gCPUAccessFlags[mBufferAccess] | D3D11_CPU_ACCESS_WRITE) && mCurrentSize == size)
	{
		ID3D11DeviceContext *context;
		mDevice->GetImmediateContext(&context);

		D3D11_BOX destBox;
		destBox.left = 0;
		destBox.right = size;
		destBox.top = 0;
		destBox.bottom = 1;
		destBox.front = 0;
		destBox.back = 1;

		context->UpdateSubresource(mBuffer.Get(), 0, &destBox, data, 0, 0);
	}
	else
	{
		RebuildBuffer(size, data);
	}
}

void DX11DataBuffer::RebuildBuffer(size_t size, const void* initialData)
{
	mBuffer = NULL;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = gBindAccessMapping[mBufferAccess];
	bufferDesc.ByteWidth = size;
	bufferDesc.BindFlags = gBufferTypeMapping[mBufferType];
	bufferDesc.StructureByteStride = mElementStride;
	bufferDesc.CPUAccessFlags = gCPUAccessFlags[mBufferAccess];
	
	D3D11_SUBRESOURCE_DATA subResourceData;
	D3D11_SUBRESOURCE_DATA* subResourceDataPtr = NULL;

	if (initialData != NULL)
	{
		subResourceData.pSysMem = initialData;
		subResourceData.SysMemPitch = 0;
		subResourceData.SysMemSlicePitch = 0;
		subResourceDataPtr = &subResourceData;
	}

	ID3D11Buffer *newBuffer;
	HRESULT result = mDevice->CreateBuffer(&bufferDesc, subResourceDataPtr, &newBuffer);

	if (FAILED(result))
	{
		throw HResultException(result, "Could not create data buffer");
	}
	
	mBuffer = newBuffer;
	mCurrentSize = size;
}

ID3D11Buffer* DX11DataBuffer::GetBuffer()
{
	return mBuffer.Get();
}

D3D11_USAGE DX11DataBuffer::gBindAccessMapping[BufferAccess::Count] = {
	D3D11_USAGE_DEFAULT,
	D3D11_USAGE_IMMUTABLE,
	D3D11_USAGE_DYNAMIC,
	D3D11_USAGE_STAGING,
};

D3D11_BIND_FLAG DX11DataBuffer::gBufferTypeMapping[DataBuffer::TypeCount] = {
	D3D11_BIND_VERTEX_BUFFER,
	D3D11_BIND_INDEX_BUFFER,
	D3D11_BIND_CONSTANT_BUFFER,
};

UINT DX11DataBuffer::gCPUAccessFlags[BufferAccess::Count] = {
	D3D11_CPU_ACCESS_WRITE,
	0,
	D3D11_CPU_ACCESS_WRITE,
	D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ
};

D3D11_MAP DX11DataBuffer::gMapMapping[BufferMapType::Count] = {
  D3D11_MAP_READ,
  D3D11_MAP_WRITE,
  D3D11_MAP_READ_WRITE,
  D3D11_MAP_WRITE_DISCARD,
  D3D11_MAP_WRITE_NO_OVERWRITE,
};

}