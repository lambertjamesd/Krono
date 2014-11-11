#pragma once

#include <memory>
#include "Core/Object.h"
#include "RenderState.h"
#include "RenderTargetDatabase.h"
#include "RenderStateParameters.h"
#include <vector>

namespace krono
{

class CompositeStage : public Resource
{
public:
	typedef std::shared_ptr<CompositeStage> Ptr;

	virtual ~CompositeStage(void);

	virtual void Render(RenderState& renderState) = 0;
	
	RenderStateParameters& GetStateParameters();

	void SetExpectedTargetInputCount(size_t value);
	size_t GetExpectedTargetInputCount() const;

	template <typename T>
	void SetVariable(const std::string& name, const T& value)
	{
		mStateParameters.SetVariable<T>(name, value);
	}
	
	template <typename T>
	void SetArray(const std::string& name, const T* value, size_t count)
	{
		mStateParameters.SetArrayVariable(name, value, count);
	}
protected:
	CompositeStage(void);

	void PushStateParameters(RenderState& renderState) const;

	size_t mExpectedTargetInputCount;
private:
	RenderStateParameters mStateParameters;
};

}
