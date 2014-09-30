
#include "GeometryCache.h"
#include <cassert>

namespace krono
{

GeometryCache::GeometryCache(Graphics& graphics) :
	mGraphics(graphics)
{

}

const Mesh::Ptr& GeometryCache::GetPlane()
{
	if (mPlane == NULL)
	{
		VertexBuffer::Ptr vertexBuffer = BuildVertexBuffer();

		DataIterator data = vertexBuffer->Lock(4);

		data.Write<GeometryVertex>(GeometryVertex(
			Vector3f(-1.0f, -1.0f, 0.0f),
			Vector3f(0.0f, 0.0f, -1.0f),
			Vector4f(1.0f, 0.0f, 0.0f, 1.0f),
			Vector2f(0.0f, 1.0f)));

		data.Write<GeometryVertex>(GeometryVertex(
			Vector3f(-1.0f, 1.0f, 0.0f),
			Vector3f(0.0f, 0.0f, -1.0f),
			Vector4f(1.0f, 0.0f, 0.0f, 1.0f),
			Vector2f(0.0f, 0.0f)));

		data.Write<GeometryVertex>(GeometryVertex(
			Vector3f(1.0f, 1.0f, 0.0f),
			Vector3f(0.0f, 0.0f, -1.0f),
			Vector4f(1.0f, 0.0f, 0.0f, 1.0f),
			Vector2f(1.0f, 0.0f)));

		data.Write<GeometryVertex>(GeometryVertex(
			Vector3f(1.0f, -1.0f, 0.0f),
			Vector3f(0.0f, 0.0f, -1.0f),
			Vector4f(1.0f, 0.0f, 0.0f, 1.0f),
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

const Mesh::Ptr& GeometryCache::GetSphere()
{
	if (mSphere == NULL)
	{
		float horizontalStep = Constant<float>::PI * 2.0f / gSphereLongitude;
		float verticalStep = Constant<float>::PI / gSphereLatitude;
		float uStep = 1.0f / gSphereLongitude;
		float vStep = 1.0f / gSphereLatitude;

		size_t vertexCount = (gSphereLongitude + 1) * (gSphereLatitude - 1) + gSphereLongitude * 2;
		
		VertexBuffer::Ptr vertexBuffer = BuildVertexBuffer();
		DataIterator vertexData = vertexBuffer->Lock(vertexCount);  
		size_t currentVertex = 0;

		float uCoordiate = horizontalStep * 0.5f;
		for (size_t x = 0; x < gSphereLongitude; ++x)
		{
			vertexData.Write<GeometryVertex>(GeometryVertex(
				Vector3f(0.0f, 1.0f, 0.0f),
				Vector3f(0.0f, 1.0f, 0.0),
				Vector4f(-Math<float>::Sin(uStep * x), 0.0f, Math<float>::Cos(uStep * x), 1.0f),
				Vector2f(uCoordiate, 0.0f)));

			uCoordiate += uStep;
			++currentVertex;
		}
		
		float latitude = verticalStep;
		float vCoordinate = 0.0f;
		for (size_t y = 1; y < gSphereLatitude; ++y)
		{
			float longitude = 0.0f;
			uCoordiate = 0.0f;
			for (size_t x = 0; x <= gSphereLongitude; ++x)
			{
				Vector3f position(
					Math<float>::Cos(longitude) * Math<float>::Sin(latitude),
					Math<float>::Cos(latitude),
					Math<float>::Sin(longitude) * Math<float>::Sin(latitude));

				GeometryVertex vertex(position, position,
					Vector4f(-Math<float>::Sin(longitude), 0.0f, Math<float>::Cos(longitude), 1.0f),
					Vector2f(uCoordiate, vCoordinate));

				vertexData.Write<GeometryVertex>(vertex);

				longitude += horizontalStep;
				uCoordiate += uStep;
				++currentVertex;
			}
			latitude += verticalStep;
			vCoordinate += vStep;
		}
		
		uCoordiate = horizontalStep * 0.5f;
		for (size_t x = 0; x < gSphereLongitude; ++x)
		{
			vertexData.Write<GeometryVertex>(GeometryVertex(
				Vector3f(0.0f, -1.0f, 0.0f),
				Vector3f(0.0f, -1.0f, 0.0),
				Vector4f(-Math<float>::Sin(uStep * x), 0.0f, Math<float>::Cos(uStep * x), 1.0f),
				Vector2f(uCoordiate, 1.0f)));

			uCoordiate += uStep;
			++currentVertex;
		}

		assert(currentVertex == vertexCount && "vertex count mismatch");

		vertexBuffer->Unlock();

		size_t indexCount = (gSphereLatitude - 1) * gSphereLongitude * 6;
		size_t currentIndex = 0;

		IndexBuffer::Ptr indexBuffer = mGraphics.CreateIndexBuffer(IndexBuffer::UInt16);
		DataIterator indexData = indexBuffer->Lock(indexCount);

		for (size_t x = 0; x < gSphereLongitude; ++x)
		{
			indexData.Write<UInt16>(gSphereLongitude + x);
			indexData.Write<UInt16>(x);
			indexData.Write<UInt16>(gSphereLongitude + x + 1);
			currentIndex += 3;
		}

		size_t rowStart = gSphereLongitude;
		size_t nextRow = rowStart + gSphereLongitude + 1;
		
		for (size_t y = 1; y < gSphereLatitude - 1; ++y)
		{
			for (size_t x = 0; x < gSphereLongitude; ++x)
			{
				indexData.Write<UInt16>(nextRow + x);
				indexData.Write<UInt16>(rowStart + x);
				indexData.Write<UInt16>(rowStart + x + 1);
				
				indexData.Write<UInt16>(nextRow + x);
				indexData.Write<UInt16>(rowStart + x + 1);
				indexData.Write<UInt16>(nextRow + x + 1);

				currentIndex += 6;
			}

			rowStart = nextRow;
			nextRow = rowStart + gSphereLongitude + 1;
		}
		
		for (size_t x = 0; x < gSphereLongitude; ++x)
		{
			indexData.Write<UInt16>(rowStart + x);
			indexData.Write<UInt16>(rowStart + x + 1);
			indexData.Write<UInt16>(nextRow + x);
			currentIndex += 3;
		}

		assert(currentIndex == indexCount && "index count mismatch");

		indexBuffer->Unlock();
		
		mSphere.reset(new Mesh());
		mSphere->AddSubMesh(vertexBuffer, indexBuffer, 0, indexCount);
	}

	return mSphere;
}

VertexBuffer::Ptr GeometryCache::BuildVertexBuffer()
{
	InputLayout inputLayout;
	inputLayout.AddAttribute(Attribute("POSITION", DataFormat(DataFormat::Float, 3)));
	inputLayout.AddAttribute(Attribute("NORMAL", DataFormat(DataFormat::Float, 3)));
	inputLayout.AddAttribute(Attribute("TANGENT", DataFormat(DataFormat::Float, 4)));
	inputLayout.AddAttribute(Attribute("TEXCOORD", DataFormat(DataFormat::Float, 2)));
	return mGraphics.CreateVertexBuffer(inputLayout);
}


GeometryCache::GeometryVertex::GeometryVertex(
	const Vector3f& position,
	const Vector3f& normal,
	const Vector4f& tangent,
	const Vector2f& textureCoord) :
	position(position),
	normal(normal),
	tangent(tangent),
	textureCoord(textureCoord)
{

}

}
