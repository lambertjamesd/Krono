#pragma once
#include "Mesh.h"
#include <map>
#include <string>
#include "Core/Memory.h"
#include "Resource.h"

class MeshGroup : public Resource
{
public:
	MeshGroup(void);
	~MeshGroup(void);

	void AddMesh(const std::string& name, Auto<Mesh>& mesh);
	Auto<Mesh>& GetMesh(const std::string& name);
private:
	std::map<std::string, Auto<Mesh> > mMeshes;
};

