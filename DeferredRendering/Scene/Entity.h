#pragma once

#include "Math/Matrix.h"
#include "Resource/Mesh/Mesh.h"
#include "Resource/Material.h"
#include "Collide/Frustrum.h"

#include <vector>

namespace krono
{

class RenderState;

class SceneIndex;

struct EntityData
{
	EntityData();

	Matrix4f modelMatrix;
	Matrix4f viewModelMatrix;
	Matrix4f projectionViewModelMatrix;

	Matrix4f normalMatrix;
};

class Entity
{
public:
	~Entity(void);

	void SetTransform(const Matrix4f& value);
	const Matrix4f& GetTransform() const;

	void SetMesh(Auto<Mesh>& mesh);
	const Auto<Mesh>& GetMesh() const;

	size_t GetMaterialCount() const;
	void SetMaterial(const Auto<Material>& material, size_t index);
	const Auto<Material>& GetMaterial(size_t index) const;

	void SetIsVisible(bool value);
	bool IsVisible() const;

	bool ShouldRender(const Frustrum& frustrum) const;

	void Render(RenderState& renderState, size_t technique);
private:
	friend class SceneIndex;
	friend class Scene;

	Entity(SceneIndex *sceneIndex);

	SceneIndex *mSceneIndex;
	void *mSceneIndexData;
	
	void RebuildUsedTechniques();
	void RebuildBuffer(RenderState& renderState);
	Auto<ConstantBuffer> mEntityBuffer;

	Auto<Mesh> mMesh;
	std::vector<Auto<Material> > mMaterials;
	EntityData mEntityData;
	bool mIsVisisble;
	Material::TechniqueMask mUsedTechniques;
};

}