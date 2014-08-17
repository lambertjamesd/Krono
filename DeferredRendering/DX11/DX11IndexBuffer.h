#pragma once

#include "DX11Common.h"

#include "Interface/IndexBuffer.h"
#include "DX11SmartPtr.h"

class DX11IndexBuffer : public IndexBuffer
{
public:
	DX11IndexBuffer(ID3D11Device *device, IndexBuffer::Format format);
	~DX11IndexBuffer(void);
	
	virtual DataIterator Lock(size_t indexCount);
	virtual void Unlock();
	
	virtual size_t GetIndexCount() const;

	DXGI_FORMAT GetDXFormat() const;

	ID3D11Buffer *GetBuffer();
public:
	void CleanUp();
	void RebuildBuffer(size_t vertexCount);

	DX11SmartPtr<ID3D11Buffer> mBuffer;
	ID3D11Device *mDevice;
	size_t mCurrentIndexCount;

	static DXGI_FORMAT gFormatMapping[];
};

