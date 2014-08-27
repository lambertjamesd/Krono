#include "Compositor.h"

#include "Core/Exception.h"

namespace krono
{

void CompositeStageConnections::AddRenderTarget(const std::string& targetName)
{
	mRenderTargets.AddRenderTarget(RenderTargetDatabase::GetTargetID(targetName));
}

void CompositeStageConnections::SetDepthBuffer(const std::string& bufferName)
{
	mRenderTargets.SetDepthTarget(RenderTargetDatabase::GetTargetID(bufferName));
}

void CompositeStageConnections::AddRenderTargetInput(const std::string& targetName)
{
	mTargetInputs.push_back(RenderTargetDatabase::GetTargetID(targetName));
}


void CompositeStageConnections::PushInputTargets(RenderState& renderState) const
{
	for (auto it = mTargetInputs.cbegin(); it != mTargetInputs.cend(); ++it)
	{
		renderState.PushRenderTargetTexture(*it, ShaderStage::PixelShader);
	}
}

const RenderTargetDescription& CompositeStageConnections::GetTargetDescription() const
{
	return mRenderTargets;
}

size_t CompositeStageConnections::GetInputCount() const
{
	return mTargetInputs.size();
}

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
	mRenderTargetDatabase.BeginCompositeRender(renderTargetConfig, graphics);

	ClearRenderTargets();

	RenderState renderState(graphics, mRenderTargetDatabase, sceneView);

	renderState.SetViewport(sceneView.CalculateViewport(renderTargetConfig.GetSize()), sceneView.GetDepthRange());

	for (size_t i = 0; i < mCompositeStages.size(); ++i)
	{
		CompositeStageEntry& entry = mCompositeStages[i];

		mRenderTargetDatabase.UseRenderTargets(entry.connections.GetTargetDescription());

		if (entry.connections.GetInputCount() != entry.compositeStage->GetExpectedTargetInputCount())
		{
			throw BadParameterException("Mismatch between number texture inputs between composite stage and connection");
		}

		renderState.PushState();
		entry.connections.PushInputTargets(renderState);

		entry.compositeStage->Render(renderState);

		renderState.PopState();
	}
}

void Compositor::AddRenderTarget(const std::string& name, const DataFormat& dataFormat)
{
	mRenderTargetDatabase.SetDataFormat(RenderTargetDatabase::GetTargetID(name), dataFormat);
}

void Compositor::AddCompositeStage(const CompositeStage::Ptr& compositeStage, const CompositeStageConnections& connections)
{
	CompositeStageEntry entry;
	entry.compositeStage = compositeStage;
	entry.connections = connections;
	mCompositeStages.push_back(entry);
}

}
