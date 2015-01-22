#pragma once

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
		Skip<T>(1);
	}
	
	template <typename T>
	void Skip(size_t count)
	{
		mDataPointer = (void*)((T*)mDataPointer + count);
	}

	bool IsFull() const;
	bool CanFit(size_t amount) const;

private:
	void* mDataStart;
	void* mDataPointer;
	void* mDataEnd;
};

namespace BufferAccess
{
	enum Type
	{
		// GPU read/write
		Default,
		// GPU read only
		Immutable,
		// GPU read CPU write
		Dynamic,
		// GPU and CPU read/write
		Staging,

		Count
	};
}

namespace BufferMapType
{
	enum Type
	{
		Read,
		Write,
		ReadWrite,
		WriteDiscard,
		WriteNoOverwrite,

		Count
	};
}

class DataBuffer
{
public:
	enum BufferType
	{
		VertexBuffer,
		IndexBuffer,
		ConstantBuffer,
		TypeCount
	};

	virtual ~DataBuffer(void);
	
	virtual DataIterator Map(size_t size, BufferMapType::Type mappingType) = 0;
	virtual void Unmap() = 0;
	virtual void Set(const void* data, size_t size) = 0;

	size_t GetCurrentSize() const;

	size_t GetElementStride() const;
	void SetElementStride(size_t value);

protected:
	DataBuffer(BufferType usage, BufferAccess::Type bufferAccess);

	BufferType mBufferType;
	BufferAccess::Type mBufferAccess;
	size_t mCurrentSize;
	size_t mElementStride;
};

}