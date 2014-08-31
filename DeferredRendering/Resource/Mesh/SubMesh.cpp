
#include "SubMesh.h"

#include "Interface/Graphics.h"

namespace krono
{

SubMesh::SubMesh(void)
{
}

SubMesh::SubMesh(Auto<VertexBuffer>& vertexBuffer, Auto<IndexBuffer>& indexBuffer) :
	mTopology(Topology::TriangleList),
	mVertexBuffer(vertexBuffer),
	mIndexBuffer(indexBuffer),
	mIndexOffset(0)
{
	if (indexBuffer == NULL)
	{
		mVertexCount = vertexBuffer->GetVertexCount();
	}
	else
	{
		mVertexCount = mIndexBuffer->GetIndexCount();
	}
}

SubMesh::SubMesh(Auto<VertexBuffer>& vertexBuffer, Auto<IndexBuffer>& indexBuffer, size_t indexOffset, size_t vertexCount) :
	mTopology(Topology::TriangleList),
	mVertexBuffer(vertexBuffer),
	mIndexBuffer(indexBuffer),
	mIndexOffset(indexOffset),
	mVertexCount(vertexCount)
{

}

SubMesh::~SubMesh(void)
{
}

void SubMesh::Render(Graphics& graphics)
{
	graphics.SetIndexBuffer(mIndexBuffer);
	graphics.SetVertexBuffer(mVertexBuffer);
	graphics.SetTopology(mTopology);

	if (mIndexBuffer == NULL)
	{
		graphics.Draw(mVertexCount, mIndexOffset);
	}
	else
	{
		graphics.DrawIndexed(mVertexCount, mIndexOffset);
	}
}

void SubMesh::SetTopology(Topology::Type topology)
{
	mTopology = topology;
}

Topology::Type SubMesh::GetTopology() const
{
	return mTopology;
}

}
