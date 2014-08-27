
#include "GeometryCache.h"
#include <cassert>

using namespace krono;

GeometryCache::GeometryCache(Graphics& graphics) :
	mGraphics(graphics)
{

}

const Mesh::Ptr& GeometryCache::GetPlane()
{
	if (mPlane == NULL)
	{
		InputLayout inputLayout;
		inputLayout.AddAttribute(Attribute("POSITION", DataFormat(DataFormat::Float, 3)));
		inputLayout.AddAttribute(Attribute("NORMAL", DataFormat(DataFormat::Float, 3)));
		inputLayout.AddAttribute(Attribute("TEXCOORD", DataFormat(DataFormat::Float, 2)));

		VertexBuffer::Ptr vertexBuffer = mGraphics.CreateVertexBuffer(inputLayout);

		DataIterator data = vertexBuffer->Lock(4);

		data.Write<GeometryVertex>(GeometryVertex(
			Vector3f(-1.0f, -1.0f, 0.0f),
			Vector3f(0.0f, 0.0f, 1.0f),
			Vector2f(0.0f, 1.0f)));

		data.Write<GeometryVertex>(GeometryVertex(
			Vector3f(-1.0f, 1.0f, 0.0f),
			Vector3f(0.0f, 0.0f, 1.0f),
			Vector2f(0.0f, 0.0f)));

		data.Write<GeometryVertex>(GeometryVertex(
			Vector3f(1.0f, 1.0f, 0.0f),
			Vector3f(0.0f, 0.0f, 1.0f),
			Vector2f(1.0f, 0.0f)));

		data.Write<GeometryVertex>(GeometryVertex(
			Vector3f(1.0f, -1.0f, 0.0f),
			Vector3f(0.0f, 0.0f, 1.0f),
			Vector2f(1.0f, 1.0f)));

		vertexBuffer->Unlock();

		IndexBuffer::Ptr indexBuffer = mGraphics.CreateIndexBuffer(IndexBuffer::UInt16);

		DataIterator indexData = indexBuffer->Lock(6);
		indexData.Write<UInt16>(0);
		indexData.Write<UInt16>(1);
		indexData.Write<UInt16>(2);

		indexData.Write<UInt16>(0);
		indexData.Write<UInt16>(2);
		indexData.Write<UInt16>(3);

		indexBuffer->Unlock();

		mPlane.reset(new Mesh());
		mPlane->AddSubMesh(vertexBuffer, indexBuffer, 0, 6);
	}

	return mPlane;
}

GeometryCache::GeometryVertex::GeometryVertex(
	const Vector3f& position,
	const Vector3f& normal,
	const Vector2f& textureCoord) :
	position(position),
	normal(normal),
	textureCoord(textureCoord)
{

}
