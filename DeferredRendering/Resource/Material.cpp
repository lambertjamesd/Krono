
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
	renderState.PushState();

	auto foundTechnique = mTechniques.find(technique);

	if (foundTechnique == mTechniques.end())
	{
		return false;
	}
	else
	{
		renderState.PushConstantBuffer(mConstantBuffer, ShaderStage::PixelShader);
		foundTechnique->second.Use(renderState);
		return true;
	}

	renderState.PopState();
}

}
