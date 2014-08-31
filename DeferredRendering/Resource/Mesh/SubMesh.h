#pragma once

#include "Interface/VertexBuffer.h"
#include "Interface/IndexBuffer.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"

#include "Resource/Material.h"

namespace krono
{

class Graphics;

class SubMesh
{
public:
	SubMesh(void);
	SubMesh(Auto<VertexBuffer>& vertexBuffer, Auto<IndexBuffer>& indexBuffer);
	SubMesh(Auto<VertexBuffer>& vertexBuffer, Auto<IndexBuffer>& indexBuffer, size_t indexOffset, size_t vertexCount);
	~SubMesh(void);

	void Render(Graphics& graphics);

	void SetTopology(Topology::Type topology);
	Topology::Type GetTopology() const;
private:
	Topology::Type mTopology;
	Auto<VertexBuffer> mVertexBuffer;
	Auto<IndexBuffer> mIndexBuffer;
	size_t mIndexOffset;
	size_t mVertexCount;
};

}
