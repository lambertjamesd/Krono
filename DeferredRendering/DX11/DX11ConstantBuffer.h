#pragma once

#include "..\Interface\ConstantBuffer.h"

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

	ID3D11Buffer *mBuffer;
	ID3D11Device *mDevice;
	size_t mCurrentSize;
};

