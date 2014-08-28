#pragma once

#include <memory>
#include "Core/Object.h"
#include "RenderState.h"
#include "RenderTargetDatabase.h"
#include "RenderStateParameters.h"
#include <vector>

namespace krono
{

class CompositeStage : public Object
{
public:
	typedef std::shared_ptr<CompositeStage> Ptr;

	virtual ~CompositeStage(void);

	virtual void Render(RenderState& renderState) = 0;
	
	RenderStateParameters& GetStateParameters();

	void SetExpectedTargetInputCount(size_t value);
	size_t GetExpectedTargetInputCount() const;
protected:
	CompositeStage(void);

	void PushStateParameters(RenderState& renderState) const;

	size_t mExpectedTargetInputCount;
private:
	RenderStateParameters mStateParameters;
};

}
