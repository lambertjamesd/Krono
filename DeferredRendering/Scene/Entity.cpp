
#include "Entity.h"
#include "Interface/Graphics.h"
#include "Compositor/RenderState.h"
#include "SceneIndex.h"
#include <assert.h>
#include <cstddef>

namespace krono
{

EntityData::EntityData() :
	modelMatrix(Matrix4f::Identity()),
	viewModelMatrix(Matrix4f::Identity()),
	projectionViewModelMatrix(Matrix4f::Identity()),
	normalMatrix(Matrix4f::Identity())
{

}

Entity::Entity(SceneIndex* sceneIndex) :
	mSceneIndex(sceneIndex),
	mIsVisisble(true),
	mUsedTechniques(0)
{
}


Entity::~Entity(void)
{
}

void Entity::SetTransform(const Matrix4f& value)
{
	mEntityData.modelMatrix = value;

	mSceneIndex->UpdateEntity(this);
}

const Matrix4f& Entity::GetTransform() const
{
	return mEntityData.modelMatrix;
}

void Entity::SetMesh(const Auto<Mesh>& mesh)
{
	mMesh = mesh;

	if (mMesh == NULL)
	{
		mMaterials.clear();
	}
	else
	{
		mMaterials.resize(mMesh->GetSubMeshCount());
	}

	RebuildUsedTechniques();
}

const Auto<Mesh>& Entity::GetMesh() const
{
	return mMesh;
}

size_t Entity::GetMaterialCount() const
{
	return mMaterials.size();
}

void Entity::SetMaterial(const Auto<Material>& material, size_t index)
{
	assert(index < mMaterials.size());
	mMaterials[index] = material;

	RebuildUsedTechniques();
}

void Entity::SetIsVisible(bool value)
{
	mIsVisisble = value;
}

bool Entity::IsVisible() const
{
	return mIsVisisble;
}

bool Entity::ShouldRender(const Frustrum& frustrum) const
{
	return mIsVisisble;
}

const Auto<Material>& Entity::GetMaterial(size_t index) const
{
	return mMaterials[index];
}

void Entity::Render(RenderState& renderState, size_t technique)
{
	if (mIsVisisble && (mUsedTechniques & (1 << technique)))
	{
		RebuildBuffer(renderState);

		renderState.PushState();
		renderState.PushConstantBuffer(mEntityBuffer, ShaderStage::VertexShader);

		for (size_t i = 0; i < mMesh->GetSubMeshCount() && i < mMaterials.size(); ++i)
		{
			if (mMaterials[i] != NULL && mMaterials[i]->HasTechnique(technique))
			{
				renderState.PushState();
				mMaterials[i]->Use(renderState, technique);
				mMesh->GetSubMesh(i)->Render(renderState.GetGraphics());
				renderState.PopState();
			}
		}

		renderState.PopState();
	}
}

void Entity::RebuildUsedTechniques()
{
	mUsedTechniques = 0;

	for (auto it = mMaterials.begin(); it != mMaterials.end(); ++it)
	{
		if (*it != NULL)
		{
			mUsedTechniques |= (*it)->GetTechniqueMask();
		}
	}
}

void Entity::RebuildBuffer(RenderState& renderState)
{
	if (mEntityBuffer == NULL)
	{
		ConstantBufferLayout layout;
		layout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(EntityData, projectionViewModelMatrix));
		mEntityBuffer = Auto<ConstantBuffer>(renderState.GetGraphics().CreateConstantBuffer(layout));
	}

	mEntityData.viewModelMatrix = renderState.GetViewMatrix() * mEntityData.modelMatrix;
	mEntityData.projectionViewModelMatrix = renderState.GetProjectionMatrix() * mEntityData.viewModelMatrix;
	mEntityData.normalMatrix = mEntityData.modelMatrix.Inverse().Transpose();

	mEntityBuffer->Set<EntityData>(mEntityData);
}

}
