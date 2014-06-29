#pragma once

class DepthBuffer
{
public:
	DepthBuffer(void);
	~DepthBuffer(void);
	
	virtual void GetRenderTargetInternal(void* target) const = 0;
private:
};

