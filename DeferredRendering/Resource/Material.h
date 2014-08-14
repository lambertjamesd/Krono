#pragma once

#include <unordered_map>
#include "Interface\ConstantBuffer.h"
#include "Technique.h"
#include "Interface/Graphics.h"

class Material
{
public:
	Material(void);
	~Material(void);

	void AddTechnique(UInt32 id, const Technique& technique);

	Auto<ConstantBuffer>& GetConstantBuffer();

	bool Use(Graphics& graphics, size_t technique);
private:
	std::unordered_map<UInt32, Technique> mTechniques;
	Auto<ConstantBuffer> mConstantBuffer;
	
	static const size_t MATERIAL_DATA_INDEX = 2;
};

