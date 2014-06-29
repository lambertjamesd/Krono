#pragma once
#include <string>
#include <vector>
#include "ShaderProgram.h"
#include "InputLayout.h"

class VertexBufferIterator
{
public:
	VertexBufferIterator(void* dataPointer, void* dataEnd);
	~VertexBufferIterator();

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

	virtual VertexBufferIterator Lock(size_t vertexCount) = 0;
	virtual void Unlock() = 0;
	virtual void Use() = 0;

	const InputLayout& GetInputLayout();
protected:
	VertexBuffer(const InputLayout& inputLayout);
	InputLayout mInputLayout;
private:
};