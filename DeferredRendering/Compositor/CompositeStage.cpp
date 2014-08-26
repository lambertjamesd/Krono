#include "CompositeStage.h"

namespace krono
{

CompositeStage::CompositeStage(void)
{
}


CompositeStage::~CompositeStage(void)
{
}

const RenderTargetDescription& CompositeStage::GetTargetDescription() const
{
	return mRenderTargetDescription;
}

void CompositeStage::AddRenderTarget(const std::string& targetName)
{
	mRenderTargetDescription.AddRenderTarget(RenderTargetDatabase::GetTargetID(targetName));
}

void CompositeStage::SetDepthBuffer(const std::string& bufferName)
{
	mRenderTargetDescription.SetDepthTarget(RenderTargetDatabase::GetTargetID(bufferName));
}

void CompositeStage::AddRenderTargetInput(const std::string& targetName)
{
	mRenderTargetInput.push_back(RenderTargetDatabase::GetTargetID(targetName));
}


void CompositeStage::PushTargetInput(RenderState& renderState) const
{
	for (auto it = mRenderTargetInput.cbegin(); it != mRenderTargetInput.cend(); ++it)
	{
		renderState.PushRenderTargetTexture(*it, ShaderStage::PixelShader);
	}
}

}
