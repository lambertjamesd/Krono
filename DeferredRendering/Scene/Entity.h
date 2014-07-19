#pragma once

#include "..\Math\Matrix.h"
#include "..\Resource\Mesh.h"

class Entity
{
public:
	Entity(void);
	~Entity(void);

	void SetTransform(const Matrix4f& value);
	const Matrix4f& GetTransform() const;

	void SetMesh(Auto<Mesh>& mesh);
	Auto<Mesh>& GetMesh();
private:
	Auto<Mesh> mMesh;
	Matrix4f mTransform;
};

