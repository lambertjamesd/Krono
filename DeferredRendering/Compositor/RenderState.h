#pragma once

#include "Interface/Shader.h"
#include "Interface/Graphics.h"
#include "Scene/SceneView.h"
#include "RenderStateParameters.h"
#include "RenderTargetDatabase.h"
#include <vector>

namespace krono
{

class RenderState
{
public:
	RenderState(Graphics& graphics, RenderTargetDatabase& targetDatabase, SceneView& sceneView);
	~RenderState(void);
	
	Graphics& GetGraphics();

	void PushState();
	void PushRenderTargetTexture(UInt32 targetID, ShaderStage::Type stage);
	void PushTexture(const Texture::Ptr& texture, ShaderStage::Type stage);
	void PushSampler(const Sampler::Ptr& buffer, ShaderStage::Type stage);
	void PushConstantBuffer(const ConstantBuffer::Ptr& sampler, ShaderStage::Type stage);
	void PushParameters(const RenderStateParameters& parameters);
	void PopState();

	const Vector2i& GetRenderTargetSize() const;

	void SetViewport(const Rectf& viewport, const Rangef& depthRange);
	Rectf GetViewport() const;

	void SetVertexShader(const VertexShader::Ptr& vertexShader);
	void SetPixelShader(const PixelShader::Ptr& pixelShader);

	void SetBlendState(const BlendState::Ptr& blendState);
	void SetDepthState(const DepthState::Ptr& depthState, UInt32 stencilReference);
	void SetRasterizerState(const RasterizerState::Ptr& rasterizerState);

	const Matrix4f& GetViewMatrix() const;
	const Matrix4f& GetProjectionMatrix() const;

	void RenderScene(size_t techniqueType);
private:
	struct SavedState
	{
		SavedState(
			size_t currentTextureCount[ShaderStage::TypeCount],
			size_t currentSamplerCount[ShaderStage::TypeCount],
			size_t currentUniformCount[ShaderStage::TypeCount],
			
			size_t viewportStackSize,
			
			size_t vertexShaderStackSize,
			size_t pixelShaderStackSize,
			size_t blendStateStackSize,
			size_t depthStateStackSize,
			size_t rasterizerStateStackSize);

		size_t textureCount[ShaderStage::TypeCount];
		size_t samplerCount[ShaderStage::TypeCount];
		size_t uniformCount[ShaderStage::TypeCount];

		size_t viewportStackSize;

		size_t vertexShaderStackSize;
		size_t pixelShaderStackSize;

		size_t blendStateStackSize;
		size_t depthStateStackSize;
		size_t rasterizerStateStackSize;
	};

	Graphics& mGraphics;
	RenderTargetDatabase& mTargetDatabase;
	SceneView& mSceneView;

	std::vector<SavedState> mSavedStates;

	size_t mCurrentTextureSlot[ShaderStage::TypeCount];
	size_t mCurrentSamplerSlot[ShaderStage::TypeCount];
	size_t mCurrentUniformBufferSlot[ShaderStage::TypeCount];

	std::vector<Rectf> mViewportStack;
	std::vector<Rangef> mDepthRangeStack;

	std::vector<VertexShader::Ptr> mVertexShaderStack;
	std::vector<PixelShader::Ptr> mPixelShaderStack;

	std::vector<BlendState::Ptr> mBlendStateStack;
	std::vector<DepthState::Ptr> mDepthStateStack;
	std::vector<UInt32> mStencilReferenceStack;;
	std::vector<RasterizerState::Ptr> mRasterizerStateStack;

	Matrix4f mProjectionMatrix;
};

}
