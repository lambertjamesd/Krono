#pragma once

#include <unordered_map>
#include "Technique.h"
#include "Resource.h"
#include "Interface/ConstantBuffer.h"
#include "Interface/Graphics.h"
#include "Interface/Texture.h"
#include <vector>
#include <utility>

class Material : public Resource
{
public:
	Material(void);
	~Material(void);

	void AddTechnique(UInt32 id, const Technique& technique);

	Auto<ConstantBuffer>& GetConstantBuffer();

	bool Use(Graphics& graphics, size_t technique);
private:
	std::unordered_map<UInt32, Technique> mTechniques;
	std::vector<std::pair<ShaderStage::Type, Auto<Texture> > > mTextures;
	Auto<ConstantBuffer> mConstantBuffer;
	
	static const size_t MATERIAL_DATA_INDEX = 2;
};

