#include "RenderState.h"
#include <cassert>

namespace krono
{

RenderState::SavedState::SavedState(
	size_t currentTextureCount[ShaderStage::TypeCount],
	size_t currentSamplerCount[ShaderStage::TypeCount],
	size_t currentUniformCount[ShaderStage::TypeCount],
			
	size_t viewportStackSize,

	size_t vertexShaderStackSize,
	size_t pixelShaderStackSize,
	size_t blendStateStackSize) :
		viewportStackSize(viewportStackSize),

		vertexShaderStackSize(vertexShaderStackSize),
		pixelShaderStackSize(pixelShaderStackSize),

		blendStateStackSize(blendStateStackSize)
{
	memcpy(textureCount, currentTextureCount, sizeof(textureCount));
	memcpy(samplerCount, currentSamplerCount, sizeof(textureCount));
	memcpy(uniformCount, currentUniformCount, sizeof(uniformCount));
}

RenderState::RenderState(Graphics& graphics, RenderTargetDatabase& targetDatabase, SceneView& sceneView) :
	mGraphics(graphics),
	mTargetDatabase(targetDatabase),
	mSceneView(sceneView),
	mProjectionMatrix(mSceneView.CalculateProjectionMatrix(mSceneView.CalculateViewport(mTargetDatabase.GetRenderSize()).size))
{
	memset(mCurrentTextureSlot, 0, sizeof(mCurrentTextureSlot));
	memset(mCurrentSamplerSlot, 0, sizeof(mCurrentSamplerSlot));
	memset(mCurrentUniformBufferSlot, 0, sizeof(mCurrentUniformBufferSlot));

	PushState();
}


RenderState::~RenderState(void)
{
	assert(mSavedStates.size() == 1 && "Missing PopState pair");
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
		
		mViewportStack.size(),

		mVertexShaderStack.size(),
		mPixelShaderStack.size(),

		mBlendStateStack.size()
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

void RenderState::PushParameters(const RenderStateParameters& parameters)
{
	for (size_t stageIndex = 0; stageIndex < ShaderStage::TypeCount; ++stageIndex)
	{
		ShaderStage::Type stage = static_cast<ShaderStage::Type>(stageIndex);
		for (size_t i = 0; i < parameters.GetTextureCount(stage); ++i)
		{
			PushTexture(parameters.GetTexture(stage, i), stage);
		}

		for (size_t i = 0; i < parameters.GetSamplerCount(stage); ++i)
		{
			PushSampler(parameters.GetSampler(stage, i), stage);
		}

		for (size_t i = 0; i < parameters.GetConstantBufferCount(stage); ++i)
		{
			PushConstantBuffer(parameters.GetConstantBuffer(stage, i), stage);
		}
	}

	if (parameters.mVertexShader != NULL)
	{
		SetVertexShader(parameters.mVertexShader);
	}

	if (parameters.mPixelShader != NULL)
	{
		SetPixelShader(parameters.mPixelShader);
	}

	if (parameters.mBlendState != NULL)
	{
		SetBlendState(parameters.mBlendState);
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

	if (savedState.vertexShaderStackSize != mVertexShaderStack.size())
	{
		mVertexShaderStack.pop_back();

		if (mVertexShaderStack.size() > 0)
		{
			mGraphics.SetVertexShader(mVertexShaderStack.back());
		}
	}

	if (savedState.pixelShaderStackSize != mPixelShaderStack.size())
	{
		mPixelShaderStack.pop_back();

		if (mPixelShaderStack.size() > 0)
		{
			mGraphics.SetPixelShader(mPixelShaderStack.back());
		}
	}

	if (savedState.blendStateStackSize != mBlendStateStack.size())
	{
		mBlendStateStack.pop_back();

		if (mBlendStateStack.size() > 0)
		{
			mGraphics.SetBlendState(mBlendStateStack.back());
		}
	}

	mSavedStates.pop_back();
}

const Vector2i& RenderState::GetRenderTargetSize() const
{
	return mTargetDatabase.GetRenderSize();
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

Rectf RenderState::GetViewport() const
{
	if (mViewportStack.size())
	{
		return mViewportStack.back();
	}
	else
	{
		return Rectf();
	}
}

void RenderState::SetVertexShader(const VertexShader::Ptr& vertexShader)
{
	if (mSavedStates.back().vertexShaderStackSize == mVertexShaderStack.size())
	{
		mVertexShaderStack.push_back(vertexShader);
	}
	else
	{
		mVertexShaderStack.back() = vertexShader;
	}

	mGraphics.SetVertexShader(vertexShader);
}

void RenderState::SetPixelShader(const PixelShader::Ptr& pixelShader)
{
	if (mSavedStates.back().pixelShaderStackSize == mPixelShaderStack.size())
	{
		mPixelShaderStack.push_back(pixelShader);
	}
	else
	{
		mPixelShaderStack.back() = pixelShader;
	}

	mGraphics.SetPixelShader(pixelShader);
}

void RenderState::SetBlendState(const BlendState::Ptr& blendState)
{
	if (mSavedStates.back().blendStateStackSize == mBlendStateStack.size())
	{
		mBlendStateStack.push_back(blendState);
	}
	else
	{
		mBlendStateStack.back() = blendState;
	}

	mGraphics.SetBlendState(blendState);
}

const Matrix4f& RenderState::GetViewMatrix() const
{
	return mSceneView.GetViewMatrix();
}

const Matrix4f& RenderState::GetProjectionMatrix() const
{
	return mProjectionMatrix;
}

void RenderState::RenderScene(size_t techniqueType)
{
	Frustrum viewFrustrum(GetProjectionMatrix());
	mSceneView.GetScene().CollideFrustrum(viewFrustrum, [&](Entity& entity) {
		entity.Render(*this, techniqueType);
	});
}

}
