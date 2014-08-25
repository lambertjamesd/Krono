#pragma once

#include "CompositeStage.h"
#include "Interface/Graphics.h"
#include "RenderTargetDatabase.h"
#include "RenderTargetConfiguration.h"
#include "Scene/SceneView.h"
#include <vector>

namespace krono
{

class Compositor
{
public:
	typedef Auto<Compositor> Ptr;

	Compositor(void);
	~Compositor(void);

	void Render(Graphics& graphics, const RenderTargetConfiguration& renderTargetConfig, SceneView& sceneView);

	void AddRenderTarget(const std::string& name, const DataFormat& dataFormat);
	void AddCompositeStage(const CompositeStage::Ptr& compositeStage);
private:
	std::vector<CompositeStage::Ptr> mCompositeStages;
	RenderTargetDatabase mRenderTargetDatabase;
};

}