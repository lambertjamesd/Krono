#pragma once
#include <string>
#include <vector>
#include "InputLayout.h"
#include "Core/Memory.h"

namespace krono
{

class DataIterator
{
public:
	DataIterator(void* dataPointer, void* dataEnd);
	~DataIterator();

	template <typename T>
	void Write(const T& value)
	{
		T* castPointer = (T*)mDataPointer;
		*castPointer = value;
		mDataPointer = (void*)(castPointer + 1);
	}

	template <typename T>
	void Write(const T* data, size_t count)
	{
		memcpy(mDataPointer, data, sizeof(T) * count);
		mDataPointer = (void*)((char*)mDataPointer + sizeof(T) * count);
	}

	template <typename T>
	void Skip()
	{
		mDataPointer = (void*)((T*)mDataPointer + 1);
	}

	bool IsFull() const;

private:
	void* mDataStart;
	void* mDataPointer;
	void* mDataEnd;
};

class VertexBuffer
{
public:
	typedef Auto<VertexBuffer> Ptr;

	virtual ~VertexBuffer(void);

	virtual DataIterator Lock(size_t vertexCount) = 0;
	virtual void Unlock() = 0;

	const InputLayout& GetInputLayout();
	
	virtual size_t GetVertexCount() const = 0;
protected:
	VertexBuffer(const InputLayout& inputLayout);
	InputLayout mInputLayout;
private:
};

}
