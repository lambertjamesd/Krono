
#include "Material.h"
#include "Compositor/RenderState.h"

namespace krono
{

Material::Material(void) :
		mTechniqueMask(0)
{
}


Material::~Material(void)
{
}

void Material::AddTechnique(UInt32 id, const Technique& technique)
{
	if (mTechniques.find(id) != mTechniques.end())
	{
		mMappedVariables.RemoveAllFrom(mTechniques[id].GetRenderStateParameters().GetMappedVariables());
	}

	mTechniques[id] = technique;
	mTechniqueMask |= 1 << id;

	mMappedVariables.AddAllFrom(mTechniques[id].GetRenderStateParameters().GetMappedVariables());
}

bool Material::HasTechnique(size_t technique)
{
	return mTechniques.find(technique) != mTechniques.end();
}

void Material::Use(RenderState& renderState, size_t technique)
{
	auto foundTechnique = mTechniques.find(technique);

	if (foundTechnique != mTechniques.end())
	{
		foundTechnique->second.Use(renderState);
	}
}

void Material::SetTexture(const Texture::Ptr& texture, size_t slot, ShaderStage::Type stage)
{
	for (auto it = mTechniques.begin(); it != mTechniques.end(); ++it)
	{
		it->second.GetRenderStateParameters().SetTexture(texture, slot, stage);
	}
}

UInt32 Material::GetTechniqueMask() const
{
	return mTechniqueMask;
}

}
