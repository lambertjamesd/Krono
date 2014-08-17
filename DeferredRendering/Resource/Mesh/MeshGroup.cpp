
#include "MeshGroup.h"


MeshGroup::MeshGroup(void)
{
}


MeshGroup::~MeshGroup(void)
{
}

void MeshGroup::AddMesh(const std::string& name, Auto<Mesh>& mesh)
{
	mMeshes[name] = mesh;
}

Auto<Mesh>& MeshGroup::GetMesh(const std::string& name)
{
	auto result = mMeshes.find(name);

	if (result == mMeshes.end())
	{
		return result->second;
	}
	else
	{
		return result->second;
	}
}