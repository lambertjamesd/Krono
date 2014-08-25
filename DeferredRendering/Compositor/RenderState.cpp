#include "RenderState.h"
#include <cassert>

namespace krono
{

RenderState::SavedState::SavedState(
	size_t currentTextureCount[ShaderStage::TypeCount],
	size_t currentSamplerCount[ShaderStage::TypeCount],
	size_t currentUniformCount[ShaderStage::TypeCount],
			
	size_t viewportStackSize) :
		viewportStackSize(viewportStackSize)
{
	memcpy(textureCount, currentTextureCount, sizeof(textureCount));
	memcpy(samplerCount, currentSamplerCount, sizeof(textureCount));
	memcpy(uniformCount, currentUniformCount, sizeof(uniformCount));
}

RenderState::RenderState(Graphics& graphics, RenderTargetDatabase& targetDatabase, SceneView& sceneView) :
	mGraphics(graphics),
	mTargetDatabase(targetDatabase),
	mSceneView(sceneView)
{
	memset(mCurrentTextureSlot, 0, sizeof(mCurrentTextureSlot));
	memset(mCurrentSamplerSlot, 0, sizeof(mCurrentSamplerSlot));
	memset(mCurrentUniformBufferSlot, 0, sizeof(mCurrentUniformBufferSlot));

	PushState();
}


RenderState::~RenderState(void)
{
	memset(mCurrentTextureSlot, 0, sizeof(mCurrentTextureSlot));
	memset(mCurrentSamplerSlot, 0, sizeof(mCurrentSamplerSlot));
	memset(mCurrentUniformBufferSlot, 0, sizeof(mCurrentUniformBufferSlot));
}

Graphics& RenderState::GetGraphics()
{
	return mGraphics;
}

void RenderState::PushState()
{
	mSavedStates.push_back(SavedState(
		mCurrentTextureSlot, 
		mCurrentSamplerSlot, 
		mCurrentUniformBufferSlot,
		
		mViewportStack.size()
		));
}

void RenderState::PushRenderTargetTexture(UInt32 targetID, ShaderStage::Type stage)
{
	PushTexture(mTargetDatabase.GetRenderTexture(targetID), stage);
}

void RenderState::PushTexture(const Texture::Ptr& texture, ShaderStage::Type stage)
{
	mGraphics.SetTexture(texture, mCurrentTextureSlot[stage], stage);
	++mCurrentTextureSlot[stage];
}

void RenderState::PushSampler(const Sampler::Ptr& sampler, ShaderStage::Type stage)
{
	mGraphics.SetSampler(sampler, mCurrentSamplerSlot[stage], stage);
	++mCurrentSamplerSlot[stage];
}

void RenderState::PushConstantBuffer(const ConstantBuffer::Ptr& buffer, ShaderStage::Type stage)
{
	mGraphics.SetConstantBuffer(buffer, mCurrentUniformBufferSlot[stage], stage);
	++mCurrentUniformBufferSlot[stage];
}

void RenderState::PushParameters(RenderStateParameters& parameters)
{
	for (size_t stageIndex = 0; stageIndex < ShaderStage::TypeCount; ++stageIndex)
	{
		ShaderStage::Type stage = static_cast<ShaderStage::Type>(stageIndex);
		for (size_t i = 0; i < parameters.GetTextureCount(stage); ++i)
		{
			PushTexture(parameters.GetTexture(stage, i), stage);
			PushConstantBuffer(parameters.GetConstantBuffer(stage, i), stage);
		}
	}
}

void RenderState::PopState()
{
	assert(mSavedStates.size() >= 2 && "State popped when none was pushed");

	SavedState& savedState = mSavedStates.back();
	
	memcpy(mCurrentTextureSlot, savedState.textureCount, sizeof(mCurrentTextureSlot));
	memcpy(mCurrentSamplerSlot, savedState.samplerCount, sizeof(mCurrentSamplerSlot));
	memcpy(mCurrentUniformBufferSlot, savedState.uniformCount, sizeof(mCurrentUniformBufferSlot));

	if (savedState.viewportStackSize != mDepthRangeStack.size())
	{
		mViewportStack.pop_back();
		mDepthRangeStack.pop_back();

		mGraphics.SetViewport(mViewportStack.back(), mDepthRangeStack.back());
	}

	mSavedStates.pop_back();
}

void RenderState::SetViewport(const Rectf& viewport, const Rangef& depthRange)
{
	if (mSavedStates.back().viewportStackSize == mViewportStack.size())
	{
		mViewportStack.push_back(viewport);
		mDepthRangeStack.push_back(depthRange);
	}
	else
	{
		mViewportStack.back() = viewport;
		mDepthRangeStack.back() = depthRange;
	}

	mGraphics.SetViewport(viewport, depthRange);
}

void RenderState::SetVertexShader(const VertexShader::Ptr& vertexShader)
{
	mGraphics.SetVertexShader(vertexShader);
}

void RenderState::SetPixelShader(const PixelShader::Ptr& pixelShader)
{
	mGraphics.SetPixelShader(pixelShader);
}

const Matrix4f& RenderState::GetViewMatrix() const
{
	return mSceneView.GetViewMatrix();
}

Matrix4f RenderState::GetProjectionMatrix() const
{
	Rectf viewport = mSceneView.CalculateViewport(mTargetDatabase.GetRenderSize());
	return mSceneView.CalculateProjectionMatrix(viewport.size);
}

void RenderState::RenderScene(size_t techniqueType)
{
	Frustrum viewFrustrum(GetProjectionMatrix());
	mSceneView.GetScene().CollideFrustrum(viewFrustrum, [&](Entity& entity) {
		entity.Render(*this, techniqueType);
	});
}

}