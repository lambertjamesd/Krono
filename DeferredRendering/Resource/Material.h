#pragma once

#include <unordered_map>
#include "Technique.h"
#include "Resource.h"
#include "Interface/ConstantBuffer.h"
#include "Interface/Graphics.h"
#include "Interface/Texture.h"
#include "Compositor/RenderStateParameters.h"
#include <vector>
#include <utility>

namespace krono
{

class RenderState;

class Material : public Resource
{
public:
	typedef Auto<Material> Ptr;
	typedef UInt32 TechniqueMask;

	Material(void);
	~Material(void);

	void AddTechnique(UInt32 id, const Technique& technique);

	bool HasTechnique(size_t technique);
	void Use(RenderState& renderState, size_t technique);

	void SetTexture(const Texture::Ptr& texture, size_t slot, ShaderStage::Type stage);

	UInt32 GetTechniqueMask() const;
private:
	TechniqueMask mTechniqueMask;

	std::unordered_map<UInt32, Technique> mTechniques;
};

}
