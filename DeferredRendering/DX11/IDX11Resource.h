#pragma once

#include "DX11Common.h"

namespace krono
{

class IDX11Resource
{
public:
	IDX11Resource();
	virtual ~IDX11Resource();

	virtual ID3D11ShaderResourceView *GetResource() = 0;
};

}
