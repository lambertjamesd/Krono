#pragma once

#include <memory>
#include "RenderState.h"
#include "RenderTargetDatabase.h"

namespace krono
{

class CompositeStage
{
public:
	typedef std::shared_ptr<CompositeStage> Ptr;

	CompositeStage(void);
	virtual ~CompositeStage(void);

	virtual void Render(RenderState& renderState) = 0;
	
	const RenderTargetDescription& GetTargetDescription() const;
private:
	RenderTargetDescription mRenderTargetDescription;
};

}