#pragma once

#include "..\Math\BoundingBox.h"
#include "SubMesh.h"
#include <vector>
#include "Resource.h"

class Mesh : public Resource
{
public:
	Mesh(void);
	~Mesh(void);

	void AddSubMesh(Auto<VertexBuffer>& vertexBuffer, Auto<IndexBuffer>& indexBuffer);
	void AddSubMesh(Auto<VertexBuffer>& vertexBuffer, Auto<IndexBuffer>& indexBuffer, size_t indexOffset, size_t vertexCount);
	SubMesh* GetSubMesh(size_t index);
	size_t GetSubMeshCount() const;
private:
	std::vector<Auto<SubMesh>> mSubMeshes;
	BoundingBoxf mBoundingBox;
};

