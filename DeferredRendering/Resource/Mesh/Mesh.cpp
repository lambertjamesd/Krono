
#include "Mesh.h"


namespace krono
{

Mesh::Mesh(void)
{
}


Mesh::~Mesh(void)
{
}

void Mesh::AddSubMesh(Auto<VertexBuffer>& vertexBuffer, Auto<IndexBuffer>& indexBuffer)
{
	mSubMeshes.push_back(Auto<SubMesh>(new SubMesh(vertexBuffer, indexBuffer)));
}

void Mesh::AddSubMesh(Auto<VertexBuffer>& vertexBuffer, Auto<IndexBuffer>& indexBuffer, size_t indexOffset, size_t vertexCount)
{
	mSubMeshes.push_back(Auto<SubMesh>(new SubMesh(vertexBuffer, indexBuffer, indexOffset, vertexCount)));
}

SubMesh* Mesh::GetSubMesh(size_t index)
{
	return mSubMeshes[index].get();
}

size_t Mesh::GetSubMeshCount() const
{
	return mSubMeshes.size();
}

}
