#pragma once

#include "CompositeStage.h"
#include "Interface/Graphics.h"
#include "RenderTargetDatabase.h"
#include "RenderTargetConfiguration.h"
#include "Scene/SceneView.h"
#include <vector>
#include <map>

namespace krono
{

class Compositor
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
	void AddCompositeStage(const CompositeStage::Ptr& compositeStage);
private:
	std::vector<CompositeStage::Ptr> mCompositeStages;
	RenderTargetDatabase mRenderTargetDatabase;

	std::map<UInt32, Colorf> mClearColors;
	std::map<UInt32, float> mClearDepths;
};

}
