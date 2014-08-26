#pragma once

#include <memory>
#include "RenderState.h"
#include "RenderTargetDatabase.h"
#include "RenderStateParameters.h"
#include <vector>

namespace krono
{

class CompositeStage
{
public:
	typedef std::shared_ptr<CompositeStage> Ptr;

	virtual ~CompositeStage(void);

	virtual void Render(RenderState& renderState) = 0;
	
	const RenderTargetDescription& GetTargetDescription() const;
	RenderStateParameters& GetStateParameters();

	void AddRenderTarget(const std::string& targetName);
	void SetDepthBuffer(const std::string& bufferName);
	void AddRenderTargetInput(const std::string& targetName);
protected:
	CompositeStage(void);

	void PushTargetInput(RenderState& renderState) const;

	std::vector<UInt32> mRenderTargetInput;
private:
	RenderTargetDescription mRenderTargetDescription;
	RenderStateParameters mStateParameters;
};

}
