#pragma once
#include "..\Interface\VertexBuffer.h"

class DX11VertexBuffer :
	public VertexBuffer
{
public:
	DX11VertexBuffer(ID3D11Device *device, const InputLayout& inputLayout);
	~DX11VertexBuffer(void);
	
	virtual VertexBufferIterator Lock(size_t vertexCount);
	virtual void Unlock(); 
	
	virtual void Use();
private:
	void CleanUp();
	void RebuildBuffer(size_t vertexCount);

	ID3D11Buffer *mBuffer;
	ID3D11Device *mDevice;
	size_t mCurrentVertexCount;
};

