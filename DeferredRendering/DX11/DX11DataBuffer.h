#pragma once

#include "DX11Common.h"
#include "DX11SmartPtr.h"
#include "Interface/DataBuffer.h"

namespace krono
{

class DX11DataBuffer :
	public DataBuffer
{
public:
	DX11DataBuffer(ID3D11Device *device, BufferType type, BufferAccess::Type bufferAccess);
	~DX11DataBuffer(void);
	
	virtual DataIterator Map(size_t size, BufferMapType::Type mappingType);
	virtual void Unmap();
	virtual void Set(const void* data, size_t size);
	
	ID3D11Buffer *GetBuffer();
private:
	void RebuildBuffer(size_t size, const void* initialData);

	DX11SmartPtr<ID3D11Buffer> mBuffer;
	ID3D11Device *mDevice;
	
	static D3D11_USAGE gBindAccessMapping[BufferAccess::Count];
	static D3D11_BIND_FLAG gBufferTypeMapping[DataBuffer::TypeCount];
	static UINT gCPUAccessFlags[BufferAccess::Count];
	static D3D11_MAP gMapMapping[BufferMapType::Count];
};

}