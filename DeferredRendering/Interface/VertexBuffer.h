#pragma once
#include <string>
#include <vector>
#include "InputLayout.h"

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
	virtual ~VertexBuffer(void);

	virtual DataIterator Lock(size_t vertexCount) = 0;
	virtual void Unlock() = 0;

	const InputLayout& GetInputLayout();
protected:
	VertexBuffer(const InputLayout& inputLayout);
	InputLayout mInputLayout;
private:
};