
#include "Entity.h"
#include "Interface/Graphics.h"
#include "SceneIndex.h"
#include <assert.h>

EntityData::EntityData() :
	viewTransform(Matrix4f::Identity())
{

namespace krono
{

EntityData::EntityData() :
	viewTransform(Matrix4f::Identity())
{

}

Entity::Entity(SceneIndex* sceneIndex) :
	mSceneIndex(sceneIndex),
	mIsVisisble(true),
	mBufferIsDirty(true)
{
}


Entity::~Entity(void)
{
}

void Entity::SetTransform(const Matrix4f& value)
{
	mEntityData.viewTransform = value;
	mBufferIsDirty = true;

	mSceneIndex->UpdateEntity(this);
}

const Matrix4f& Entity::GetTransform() const
{
	return mEntityData.viewTransform;
}

void Entity::SetMesh(Auto<Mesh>& mesh)
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
}

Auto<Mesh>& Entity::GetMesh()
{
	return mMesh;
}

size_t Entity::GetMaterialCount() const
{
	return mMaterials.size();
}

void Entity::SetMaterial(Auto<Material>& material, size_t index)
{
	assert(index < mMaterials.size());
	mMaterials[index] = material;
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

void Entity::Render(Graphics& graphics, size_t technique)
{
	if (mIsVisisble)
	{
		RebuildBuffer(graphics);
		graphics.SetConstantBuffer(mEntityBuffer, ENTITY_DATA_INDEX);

		for (size_t i = 0; i < mMesh->GetSubMeshCount() && i < mMaterials.size(); ++i)
		{
			if (mMaterials[i] != NULL && mMaterials[i]->Use(graphics, technique))
			{
				mMesh->GetSubMesh(i)->Render(graphics);
			}
		}
	}
}

void Entity::RebuildBuffer(Graphics& graphics)
{
	if (mEntityBuffer == NULL)
	{
		ConstantBufferLayout layout;
		mEntityBuffer = Auto<ConstantBuffer>(graphics.CreateConstantBuffer(layout));
	}

	if (mBufferIsDirty)
	{
		mEntityBuffer->Set<EntityData>(mEntityData);
		mBufferIsDirty = false;
	}
}

}