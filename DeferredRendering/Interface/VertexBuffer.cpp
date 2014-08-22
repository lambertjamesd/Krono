
#include "VertexBuffer.h"


namespace krono
{

DataIterator::DataIterator(void* dataPointer, void* dataEnd) :
	mDataStart(dataPointer),
	mDataPointer(dataPointer),
	mDataEnd(dataEnd)
{

}

DataIterator::~DataIterator()
{

}

bool DataIterator::IsFull() const
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

}
