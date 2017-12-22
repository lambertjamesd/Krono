#include "DataBuffer.h"

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

bool DataIterator::CanFit(size_t amount) const
{
	return (size_t)((char*)mDataEnd - (char*)mDataPointer) >= amount;
}

DataBuffer::DataBuffer(BufferType type, BufferAccess::Type bufferAccess) :
	mBufferType(type),
	mBufferAccess(bufferAccess),
	mCurrentSize(0),
	mElementStride(0)
{

}


DataBuffer::~DataBuffer(void)
{
}

size_t DataBuffer::GetCurrentSize() const
{
	return mCurrentSize;
}

size_t DataBuffer::GetElementStride() const
{
	return mElementStride;
}

void DataBuffer::SetElementStride(size_t value)
{
	mElementStride = value;
}

}