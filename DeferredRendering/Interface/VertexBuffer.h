#pragma once
#include <string>
#include <vector>
#include "InputLayout.h"
#include "Core/Memory.h"
#include "DataBuffer.h"

namespace krono
{

namespace Topology
{
	enum Type
	{
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		TriangleStrip,

		Count
	};
}

class VertexBuffer
{
public:
	typedef Auto<VertexBuffer> Ptr;

	virtual ~VertexBuffer(void);

	virtual DataIterator Map(size_t vertexCount, BufferMapType::Type mappingType);
	virtual void Unmap();
	virtual void Set(const void* data, size_t vertexCount);
	size_t GetVertexCount() const;

	const InputLayout& GetInputLayout();
	DataBuffer& GetBuffer();
	
	VertexBuffer(const InputLayout& inputLayout, std::unique_ptr<DataBuffer>& buffer);
protected:
	InputLayout mInputLayout;
	std::unique_ptr<DataBuffer> mBuffer;
private:
};

}
