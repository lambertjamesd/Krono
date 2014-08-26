#include "Compositor.h"


namespace krono
{

Compositor::Compositor(void)
{
}


Compositor::~Compositor(void)
{
}

void Compositor::ClearRenderTargets()
{
	for (auto it = mClearColors.begin(); it != mClearColors.end(); ++it)
	{
		mRenderTargetDatabase.ClearRenderTarget(it->first, it->second);
	}

	for (auto it = mClearDepths.begin(); it != mClearDepths.end(); ++it)
	{
		mRenderTargetDatabase.ClearDepthBuffer(it->first, it->second);
	}
}

void Compositor::SetClearColor(const std::string& targetName, const Colorf& color)
{
	mClearColors[RenderTargetDatabase::GetTargetID(targetName)] = color;
}

void Compositor::SetClearDepth(const std::string& targetName, float depth)
{
	mClearDepths[RenderTargetDatabase::GetTargetID(targetName)] = depth;
}

void Compositor::Render(Graphics& graphics, const RenderTargetConfiguration& renderTargetConfig, SceneView& sceneView)
{
	mRenderTargetDatabase.BeginCompositeRender(renderTargetConfig.GetSize(), graphics);

	ClearRenderTargets();

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
