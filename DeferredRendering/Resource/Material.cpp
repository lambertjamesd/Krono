
#include "Material.h"

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

bool Material::Use(Graphics& graphics, size_t technique)
{
	auto foundTechnique = mTechniques.find(technique);

	if (foundTechnique == mTechniques.end())
	{
		return false;
	}
	else
	{
		graphics.SetConstantBuffer(mConstantBuffer, MATERIAL_DATA_INDEX);
		foundTechnique->second.Use(graphics);
		return true;
	}
}

}
