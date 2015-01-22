
#include "VertexBuffer.h"


namespace krono
{

VertexBuffer::VertexBuffer(const InputLayout& inputLayout, std::unique_ptr<DataBuffer>& buffer) :
	mInputLayout(inputLayout),
	mBuffer(move(buffer))
{
}


VertexBuffer::~VertexBuffer(void)
{
}

DataIterator VertexBuffer::Map(size_t vertexCount, BufferMapType::Type mappingType)
{
	return mBuffer->Map(vertexCount * mInputLayout.GetStride(), mappingType);
}

void VertexBuffer::Unmap()
{
	mBuffer->Unmap();
}

void VertexBuffer::Set(const void* data, size_t vertexCount)
{
	mBuffer->Set(data, vertexCount * mInputLayout.GetStride());
}

size_t VertexBuffer::GetVertexCount() const
{
	return mBuffer->GetCurrentSize() / mInputLayout.GetStride();
}

const InputLayout& VertexBuffer::GetInputLayout()
{
	return mInputLayout;
}

DataBuffer& VertexBuffer::GetBuffer()
{
	return *mBuffer;
}

}
