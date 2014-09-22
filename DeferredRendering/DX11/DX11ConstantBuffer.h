#pragma once

#include "DX11Common.h"

#include "Interface/ConstantBuffer.h"

namespace krono
{

class DX11ConstantBuffer : public ConstantBuffer
{
public:
	DX11ConstantBuffer(ID3D11Device* device, const ConstantBufferLayout& layout);
	~DX11ConstantBuffer(void);
	
	virtual void Set(const void* data, size_t size);

	ID3D11Buffer* GetBuffer();
private:
	void CleanUp();
	void Resize(size_t newSize);

	static size_t AlignSize(size_t size);
	static const size_t AlignmentSize = 16;

	ID3D11Buffer *mBuffer;
	ID3D11Device *mDevice;
	size_t mCurrentSize;
};

}
