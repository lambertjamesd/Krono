#pragma once

#include "Math/Matrix.h"
#include "Resource/Mesh/Mesh.h"
#include "Resource/Material.h"
#include "Collide/Frustrum.h"

#include <vector>

class SceneIndex;

struct EntityData
{
	EntityData();

	Matrix4f viewTransform;
};

class Entity
{
public:
	~Entity(void);

	void SetTransform(const Matrix4f& value);
	const Matrix4f& GetTransform() const;

	void SetMesh(Auto<Mesh>& mesh);
	Auto<Mesh>& GetMesh();

	size_t GetMaterialCount() const;
	void SetMaterial(Auto<Material>& material, size_t index);
	const Auto<Material>& GetMaterial(size_t index) const;

	void SetIsVisible(bool value);
	bool IsVisible() const;

	bool ShouldRender(const Frustrum& frustrum) const;

	void Render(Graphics& graphics, size_t technique);
private:
	friend class SceneIndex;
	friend class Scene;

	Entity(SceneIndex *sceneIndex);

	SceneIndex *mSceneIndex;
	void *mSceneIndexData;

	void RebuildBuffer(Graphics& graphics);
	Auto<ConstantBuffer> mEntityBuffer;

	Auto<Mesh> mMesh;
	std::vector<Auto<Material> > mMaterials;
	EntityData mEntityData;
	bool mIsVisisble;

	static const size_t ENTITY_DATA_INDEX = 1;

	bool mBufferIsDirty;
};

