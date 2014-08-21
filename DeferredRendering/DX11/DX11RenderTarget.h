#pragma once

#include "DX11Common.h"

class DX11RenderTarget
{
public:
	DX11RenderTarget(void);
	~DX11RenderTarget(void);

	virtual ID3D11RenderTargetView* GetTargetView() = 0;
};

