#pragma once

#include "Collide/BoundingBox.h"
#include "SubMesh.h"
#include <vector>
#include "Resource/Resource.h"

namespace krono
{

class Mesh : public Resource
{
public:
	typedef Auto<Mesh> Ptr;

	Mesh(void);
	virtual ~Mesh(void);

	void AddSubMesh(Auto<VertexBuffer>& vertexBuffer, Auto<IndexBuffer>& indexBuffer);
	void AddSubMesh(Auto<VertexBuffer>& vertexBuffer, Auto<IndexBuffer>& indexBuffer, size_t indexOffset, size_t vertexCount);
	SubMesh* GetSubMesh(size_t index);
	size_t GetSubMeshCount() const;

	const BoundingBoxf& GetBoundingBox() const;
	void SetBoundingBox(const BoundingBoxf& boundingBox);
private:
	std::vector<Auto<SubMesh>> mSubMeshes;
	BoundingBoxf mBoundingBox;
};

}
