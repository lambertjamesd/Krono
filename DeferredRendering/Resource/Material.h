#pragma once

#include <unordered_map>
#include "..\Interface\ConstantBuffer.h"
#include "Technique.h"

class Material
{
public:
	Material(void);
	~Material(void);

	void AddTechnique(UINT32 id, const Technique& technique);

	Auto<ConstantBuffer>& GetConstantBuffer();
private:
	std::unordered_map<UINT32, Technique> mTechniques;
	Auto<ConstantBuffer> mConstantBuffer;
};

