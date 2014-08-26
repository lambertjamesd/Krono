
#include "Material.h"
#include "Compositor/RenderState.h"

namespace krono
{

Material::Material(void)
{
}


Material::~Material(void)
{
}

void Material::AddTechnique(UInt32 id, const Technique& technique)
{
	mTechniques[id] = technique;
}

Auto<ConstantBuffer>& Material::GetConstantBuffer()
{
	return mConstantBuffer;
}

bool Material::Use(RenderState& renderState, size_t technique)
{

	auto foundTechnique = mTechniques.find(technique);

	bool result;

	if (foundTechnique == mTechniques.end())
	{
		result = false;
	}
	else
	{
		renderState.PushState();

		renderState.PushParameters(mRenderStateParameters);

		renderState.PushConstantBuffer(mConstantBuffer, ShaderStage::PixelShader);
		foundTechnique->second.Use(renderState);
		result = true;

		renderState.PopState();
	}


	return result;
}

void Material::SetTexture(const Texture::Ptr& texture, size_t slot, ShaderStage::Type stage)
{
	mRenderStateParameters.SetTexture(texture, slot, stage);
}

}
