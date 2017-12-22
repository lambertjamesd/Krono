#include "DeferredCompositor.h"

namespace krono {
	DeferredCompositor::DeferredCompositor(Graphics& graphics, ResourceManager& resourceManager) :
		mRenderScene(RenderSceneCompositeStage::RENDER_GEOMETRY_TECHNIQUE),
		mRenderLighting(1),
		mToneMapper(resourceManager.GetPlane())
	{
		mRenderTargetDatabase.SetDataFormat(COLOR_TARGET, DataFormat(DataFormat::Int8UNorm, 4));
		mRenderTargetDatabase.SetDataFormat(NORMAL_TARGET, DataFormat(DataFormat::Float, 4));
		mRenderTargetDatabase.SetDataFormat(SPECULAR_TARGET, DataFormat(DataFormat::Int8UNorm, 4));
		mRenderTargetDatabase.SetDataFormat(SSAO_TARGET, DataFormat(DataFormat::Int8UNorm, 1));
		mRenderTargetDatabase.SetTargetScale(SSAO_TARGET, Vector2f(0.5f, 0.5f));
		mRenderTargetDatabase.SetDataFormat(SSAO_BLUR_TARGET, DataFormat(DataFormat::Int8UNorm, 1));
		mRenderTargetDatabase.SetTargetScale(SSAO_BLUR_TARGET, Vector2f(0.5f, 0.5f));
		mRenderTargetDatabase.SetDataFormat(COMBINED_TARGET, DataFormat(DataFormat::Float, 4));

		mRenderTargetDatabase.SetDataFormat(DEPTH_TARGET, DataFormat(DataFormat::Depth32F, 1));

		{
			RenderStateParameters& stateParameters = mRenderScene.GetStateParameters();
			BlendStateDescription blendState;
			stateParameters.SetBlendState(graphics.CreateBlendState(blendState));

			DepthStateDescription depthState;
			stateParameters.SetDepthState(graphics.CreateDepthState(depthState));

			RasterizerStateDescription rasterState;
			stateParameters.SetRasterizerState(graphics.CreateRasterizerState(rasterState));
		}

		{
			mRenderLighting.SetExpectedTargetInputCount(4);

			RenderStateParameters& stateParameters = mRenderLighting.GetStateParameters();
			BlendStateDescription blendState;
			blendState.renderTargets[0] = RenderTargetBlend::Add;
			stateParameters.SetBlendState(graphics.CreateBlendState(blendState));

			DepthStateDescription depthState;
			depthState.depthEnable = false;
			stateParameters.SetDepthState(graphics.CreateDepthState(depthState));

			RasterizerStateDescription rasterState;
			rasterState.cullMode = CullMode::Front;
			stateParameters.SetRasterizerState(graphics.CreateRasterizerState(rasterState));
		}

		{
			mToneMapper.SetExpectedTargetInputCount(1);

			RenderStateParameters& stateParameters = mToneMapper.GetStateParameters();

			stateParameters.SetVertexShader(resourceManager.LoadResource<VertexShader>("Media/Shaders/Bundle/ScreenComposite.shader"));
			stateParameters.SetPixelShader(resourceManager.LoadResource<PixelShader>("Media/Shaders/Bundle/ColorMapper.shader"));

			BlendStateDescription blendState;
			stateParameters.SetBlendState(graphics.CreateBlendState(blendState));

			DepthStateDescription depthState;
			depthState.depthEnable = false;
			stateParameters.SetDepthState(graphics.CreateDepthState(depthState));

			RasterizerStateDescription rasterState;
			stateParameters.SetRasterizerState(graphics.CreateRasterizerState(rasterState));
		}
	}


	DeferredCompositor::~DeferredCompositor()
	{
	}

	void DeferredCompositor::Render(Graphics& graphics, const RenderTargetConfiguration& renderTargetConfig, SceneView& sceneView)
	{
		mRenderTargetDatabase.InitializeGraphics(graphics, renderTargetConfig.GetSize());

		Auto<RenderTarget> output = renderTargetConfig.GetRenderTargets()[0];

		mRenderTargetDatabase.GetRenderTarget(COMBINED_TARGET)->Clear(Colorf(0.0f, 1.0f, 0.0f));
		mRenderTargetDatabase.GetDepthBuffer(DEPTH_TARGET)->Clear(1.0f);

		output->Clear(Colorf(0.0f, 0.1f, 0.0f));

		RenderState renderState(graphics, mRenderTargetDatabase, sceneView);
		
		{
			std::vector<Auto<RenderTarget>> renderStageTargets = {
				mRenderTargetDatabase.GetRenderTarget(COLOR_TARGET),
				mRenderTargetDatabase.GetRenderTarget(NORMAL_TARGET),
				mRenderTargetDatabase.GetRenderTarget(SPECULAR_TARGET),
				mRenderTargetDatabase.GetRenderTarget(COMBINED_TARGET)
			};

			Auto<DepthBuffer> depthBuffer = mRenderTargetDatabase.GetDepthBuffer(DEPTH_TARGET);

			mRenderTargetDatabase.SetCurrentTargetSize(renderTargetConfig.GetSize());
			graphics.SetRenderTargets(renderStageTargets, depthBuffer);
			renderState.SetViewport(sceneView.CalculateViewport(renderTargetConfig.GetSize()), sceneView.GetDepthRange());

			renderState.PushState();
			mRenderScene.Render(renderState);
			renderState.PopState();
		}

		{
			std::vector<Auto<RenderTarget>> renderStageTargets = {
				mRenderTargetDatabase.GetRenderTarget(COMBINED_TARGET)
			};

			mRenderTargetDatabase.SetCurrentTargetSize(renderTargetConfig.GetSize());
			graphics.SetRenderTargets(renderStageTargets, NULL);
			renderState.SetViewport(sceneView.CalculateViewport(renderTargetConfig.GetSize()), sceneView.GetDepthRange());

			renderState.PushState();
			renderState.PushRenderTargetTexture(COLOR_TARGET, ShaderStage::PixelShader);
			renderState.PushRenderTargetTexture(NORMAL_TARGET, ShaderStage::PixelShader);
			renderState.PushRenderTargetTexture(DEPTH_TARGET, ShaderStage::PixelShader);
			renderState.PushRenderTargetTexture(SPECULAR_TARGET, ShaderStage::PixelShader);
			mRenderLighting.Render(renderState);
			renderState.PopState();
		}

		{
			std::vector<Auto<RenderTarget>> renderStageTargets = {
				output
			};

			mRenderTargetDatabase.SetCurrentTargetSize(renderTargetConfig.GetSize());
			graphics.SetRenderTargets(renderStageTargets, NULL);
			renderState.SetViewport(sceneView.CalculateViewport(renderTargetConfig.GetSize()), sceneView.GetDepthRange());

			renderState.PushState();
			renderState.PushRenderTargetTexture(COMBINED_TARGET, ShaderStage::PixelShader);
			mToneMapper.Render(renderState);
			renderState.PopState();

		}
	}
}

