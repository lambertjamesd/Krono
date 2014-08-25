#include "Compositor.h"


namespace krono
{

Compositor::Compositor(void)
{
}


Compositor::~Compositor(void)
{
}

void Compositor::Render(Graphics& graphics, const RenderTargetConfiguration& renderTargetConfig, SceneView& sceneView)
{
	mRenderTargetDatabase.BeginCompositeRender(renderTargetConfig.GetSize(), graphics);
	RenderState renderState(graphics, mRenderTargetDatabase, sceneView);

	renderState.SetViewport(sceneView.CalculateViewport(renderTargetConfig.GetSize()), sceneView.GetDepthRange());

	for (size_t i = 0; i < mCompositeStages.size(); ++i)
	{
		if (i + 1 == mCompositeStages.size())
		{
			// Use the supplied render targets for the last render stage
			graphics.SetRenderTargets(renderTargetConfig.GetRenderTargets(), renderTargetConfig.GetDepthBuffer());
		}
		else
		{
			mRenderTargetDatabase.UseRenderTargets(mCompositeStages[i]->GetTargetDescription());
		}

		mCompositeStages[i]->Render(renderState);
	}
}

void Compositor::AddRenderTarget(const std::string& name, const DataFormat& dataFormat)
{
	mRenderTargetDatabase.SetDataFormat(RenderTargetDatabase::GetTargetID(name), dataFormat);
}

void Compositor::AddCompositeStage(const CompositeStage::Ptr& compositeStage)
{
	mCompositeStages.push_back(compositeStage);
}

}