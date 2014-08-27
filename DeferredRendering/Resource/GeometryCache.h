#pragma once

#include "Mesh/Mesh.h"
#include "Math/Vector3.h"
#include "Math/Vector2.h"

class GeometryCache
{
public:
	GeometryCache(krono::Graphics& graphics);

	const krono::Mesh::Ptr& GetPlane();
	const krono::Mesh::Ptr& GetSphere(size_t horizontalRes, size_t verticalRes);
private:
	struct GeometryVertex
	{
		GeometryVertex(
			const krono::Vector3f& position,
			const krono::Vector3f& normal,
			const krono::Vector2f& textureCoord);

		krono::Vector3f position;
		krono::Vector3f normal;
		krono::Vector2f textureCoord;
	};

	krono::Graphics &mGraphics;

	krono::Mesh::Ptr mPlane;
};

