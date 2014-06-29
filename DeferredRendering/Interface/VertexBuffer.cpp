#include "stdafx.h"
#include "VertexBuffer.h"

VertexBufferIterator::VertexBufferIterator(void* dataPointer, void* dataEnd) :
	mDataStart(dataPointer),
	mDataPointer(dataPointer),
	mDataEnd(dataEnd)
{

}

VertexBufferIterator::~VertexBufferIterator()
{

}

bool VertexBufferIterator::IsFull() const
{
	return mDataPointer >= mDataEnd;
}

VertexBuffer::VertexBuffer(const InputLayout& inputLayout) :
	mInputLayout(inputLayout)
{
}


VertexBuffer::~VertexBuffer(void)
{
}

const InputLayout& VertexBuffer::GetInputLayout()
{
	return mInputLayout;
}