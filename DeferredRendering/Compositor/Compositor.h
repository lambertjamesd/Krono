#pragma once

#include "Core/Object.h"
#include "CompositeStage.h"
#include "Interface/Graphics.h"
#include "RenderTargetDatabase.h"
#include "RenderTargetConfiguration.h"
#include "Scene/SceneView.h"
#include "RenderState.h"
#include <vector>
#include <map>

namespace krono
{

class CompositeStageConnections
{
public:
	void AddRenderTarget(const std::string& targetName);
	void SetDepthBuffer(const std::string& bufferName);
	void AddRenderTargetInput(const std::string& targetName);

	void PushInputTargets(RenderState& renderState) const;
	const RenderTargetDescription& GetTargetDescription() const;

	size_t GetInputCount() const;
private:
	RenderTargetDescription mRenderTargets;

	std::vector<UInt32> mTargetInputs;
};

class Compositor : public Object
{
public:
	typedef Auto<Compositor> Ptr;

	Compositor(void);
	~Compositor(void);

	void ClearRenderTargets();

	void SetClearColor(const std::string& targetName, const Colorf& color);
	void SetClearDepth(const std::string& targetName, float depth);

	void Render(Graphics& graphics, const RenderTargetConfiguration& renderTargetConfig, SceneView& sceneView);

	void AddRenderTarget(const std::string& name, const DataFormat& dataFormat);
	void SetRenderTargetScale(const std::string& name, const Vector2f& scale);
	void AddCompositeStage(const CompositeStage::Ptr& compositeStage, const CompositeStageConnections& connections);

	static Ptr Null;

private:
	struct CompositeStageEntry
	{
		CompositeStage::Ptr compositeStage;
		CompositeStageConnections connections;
	};

	std::vector<CompositeStageEntry> mCompositeStages;
	RenderTargetDatabase mRenderTargetDatabase;

	std::map<UInt32, Colorf> mClearColors;
	std::map<UInt32, float> mClearDepths;
};

}
