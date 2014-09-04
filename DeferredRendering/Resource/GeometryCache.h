#pragma once

#include "Mesh/Mesh.h"
#include "Math/Vector3.h"
#include "Math/Vector2.h"

namespace krono
{

class GeometryCache
{
public:
	GeometryCache(krono::Graphics& graphics);

	const Mesh::Ptr& GetPlane();
	const Mesh::Ptr& GetSphere();
private:
	VertexBuffer::Ptr BuildVertexBuffer();

	struct GeometryVertex
	{
		GeometryVertex(
			const Vector3f& position,
			const Vector3f& normal,
			const Vector2f& textureCoord);

		Vector3f position;
		Vector3f normal;
		Vector2f textureCoord;
	};

	static const size_t gSphereLatitude = 16;
	static const size_t gSphereLongitude = 32;

	Graphics &mGraphics;

	Mesh::Ptr mPlane;
	Mesh::Ptr mSphere;
};

}

