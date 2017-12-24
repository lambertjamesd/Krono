#include "DeferredCompositor.h"

namespace krono {
	DeferredCompositor::DeferredCompositor(Graphics& graphics, ResourceManager& resourceManager) :
		mRenderScene(RenderSceneCompositeStage::RENDER_GEOMETRY_TECHNIQUE),
		mRenderLighting(1),
		mToneMapper(resourceManager.GetPlane()),
		mSSAO(resourceManager.GetPlane()),
		mSSAOBlur(resourceManager.GetPlane()),
		mAmbientLighting(resourceManager.GetPlane())
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

		{
			mSSAO.SetExpectedTargetInputCount(2);

			RenderStateParameters& stateParameters = mSSAO.GetStateParameters();

			PixelShader::Ptr pixelShader = resourceManager.LoadResource<PixelShader>("Engine/Compositor/SSAO/SSAO.shader");

			stateParameters.SetVertexShader(resourceManager.LoadResource<VertexShader>("Engine/Shaders/CompositorVertexShader.shader"));
			stateParameters.SetPixelShader(pixelShader);

			BlendStateDescription blendState;
			stateParameters.SetBlendState(graphics.CreateBlendState(blendState));

			DepthStateDescription depthState;
			depthState.depthEnable = false;
			stateParameters.SetDepthState(graphics.CreateDepthState(depthState));

			RasterizerStateDescription rasterState;
			stateParameters.SetRasterizerState(graphics.CreateRasterizerState(rasterState));

			ConstantBufferLayout layout;
			MappedConstantBuffer::Ptr constantBuffer(new MappedConstantBuffer(resourceManager.GetGraphics()->CreateConstantBuffer(layout), *pixelShader->GetInputLayout().GetUniformBlock(1)));

			stateParameters.AddConstantBuffer(constantBuffer, ShaderStage::PixelShader);
			stateParameters.AddTexture(resourceManager.LoadResource<Texture2D>("Engine/Textures/Noise.png"), ShaderStage::PixelShader);

			stateParameters.SetVariable<int>("numberOfSamples", 16);
			stateParameters.SetVariable<float>("maxRadius", 0.25f);
		}

		{
			mSSAOBlur.SetExpectedTargetInputCount(2);

			RenderStateParameters& stateParameters = mSSAOBlur.GetStateParameters();

			PixelShader::Ptr pixelShader = resourceManager.LoadResource<PixelShader>("Engine/Compositor/Blur/SpacialBlur.shader");

			stateParameters.SetVertexShader(resourceManager.LoadResource<VertexShader>("Engine/Shaders/CompositorVertexShader.shader"));
			stateParameters.SetPixelShader(pixelShader);

			BlendStateDescription blendState;
			stateParameters.SetBlendState(graphics.CreateBlendState(blendState));

			DepthStateDescription depthState;
			depthState.depthEnable = false;
			stateParameters.SetDepthState(graphics.CreateDepthState(depthState));

			RasterizerStateDescription rasterState;
			stateParameters.SetRasterizerState(graphics.CreateRasterizerState(rasterState));

			ConstantBufferLayout layout;
			MappedConstantBuffer::Ptr constantBuffer(new MappedConstantBuffer(resourceManager.GetGraphics()->CreateConstantBuffer(layout), *pixelShader->GetInputLayout().GetUniformBlock(1)));

			stateParameters.AddConstantBuffer(constantBuffer, ShaderStage::PixelShader);

			stateParameters.SetVariable<int>("maxSampleCount", 8);
			stateParameters.SetVariable<float>("maxDistance", 0.25f);
		}

		{
			mAmbientLighting.SetExpectedTargetInputCount(3);

			RenderStateParameters& stateParameters = mAmbientLighting.GetStateParameters();

			PixelShader::Ptr pixelShader = resourceManager.LoadResource<PixelShader>("Engine/Compositor/HemisphericLighting/HemisphericLighting.shader");

			stateParameters.SetVertexShader(resourceManager.LoadResource<VertexShader>("Media/Shaders/Bundle/ScreenComposite.shader"));
			stateParameters.SetPixelShader(pixelShader);
			
			BlendStateDescription blendState;
			blendState.renderTargets[0] = RenderTargetBlend::Add;
			stateParameters.SetBlendState(graphics.CreateBlendState(blendState));

			DepthStateDescription depthState;
			depthState.depthEnable = false;
			stateParameters.SetDepthState(graphics.CreateDepthState(depthState));

			RasterizerStateDescription rasterState;
			stateParameters.SetRasterizerState(graphics.CreateRasterizerState(rasterState));
			
			ConstantBufferLayout layout;
			MappedConstantBuffer::Ptr constantBuffer(new MappedConstantBuffer(resourceManager.GetGraphics()->CreateConstantBuffer(layout), *pixelShader->GetInputLayout().GetUniformBlock(1)));

			stateParameters.AddConstantBuffer(constantBuffer, ShaderStage::PixelShader);

			stateParameters.SetVariable<Colorf>("skyColor", Colorf(0.4f, 0.3f, 0.3f));
			stateParameters.SetVariable<Colorf>("groundColor", Colorf(0.3f, 0.4f, 0.3f));
			stateParameters.SetVariable<Vector3f>("up", Vector3f(1.0f, 0.0f, 0.0f));
		}
	}


	DeferredCompositor::~DeferredCompositor()
	{
	}

	void DeferredCompositor::Render(Graphics& graphics, const RenderTargetConfiguration& renderTargetConfig, SceneView& sceneView)
	{
		mRenderTargetDatabase.InitializeGraphics(graphics, renderTargetConfig.GetSize());

		Auto<RenderTarget> output = renderTargetConfig.GetRenderTargets()[0];

		mRenderTargetDatabase.GetRenderTarget(COMBINED_TARGET)->Clear(Colorf(0.0f, 0.0f, 0.0f));
		mRenderTargetDatabase.GetDepthBuffer(DEPTH_TARGET)->Clear(1.0f);

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
			auto targetSize =  mRenderTargetDatabase.GetRenderTarget(SSAO_TARGET)->GetSize();
			std::vector<Auto<RenderTarget>> renderStageTargets = {
				mRenderTargetDatabase.GetRenderTarget(SSAO_TARGET),
			};

			mRenderTargetDatabase.SetCurrentTargetSize(targetSize);
			graphics.SetRenderTargets(renderStageTargets, NULL);
			renderState.SetViewport(sceneView.CalculateViewport(targetSize), sceneView.GetDepthRange());

			renderState.PushState();
			renderState.PushRenderTargetTexture(DEPTH_TARGET, ShaderStage::PixelShader);
			renderState.PushRenderTargetTexture(NORMAL_TARGET, ShaderStage::PixelShader);
			mSSAO.Render(renderState);
			renderState.PopState();

		}

		{
			auto targetSize = mRenderTargetDatabase.GetRenderTarget(SSAO_BLUR_TARGET)->GetSize();
			std::vector<Auto<RenderTarget>> renderStageTargets = {
				mRenderTargetDatabase.GetRenderTarget(SSAO_BLUR_TARGET),
			};

			mRenderTargetDatabase.SetCurrentTargetSize(targetSize);
			graphics.SetRenderTargets(renderStageTargets, NULL);
			renderState.SetViewport(sceneView.CalculateViewport(targetSize), sceneView.GetDepthRange());

			renderState.PushState();
			renderState.PushRenderTargetTexture(SSAO_TARGET, ShaderStage::PixelShader);
			renderState.PushRenderTargetTexture(DEPTH_TARGET, ShaderStage::PixelShader);
			mSSAOBlur.GetStateParameters().SetVariable<Vector2f>("direction", Vector2f(1.0f, 0.0f));
			mSSAOBlur.Render(renderState);
			renderState.PopState();
		}

		{
			auto targetSize = mRenderTargetDatabase.GetRenderTarget(SSAO_TARGET)->GetSize();
			std::vector<Auto<RenderTarget>> renderStageTargets = {
				mRenderTargetDatabase.GetRenderTarget(SSAO_TARGET),
			};

			mRenderTargetDatabase.SetCurrentTargetSize(targetSize);
			graphics.SetRenderTargets(renderStageTargets, NULL);
			renderState.SetViewport(sceneView.CalculateViewport(targetSize), sceneView.GetDepthRange());

			renderState.PushState();
			renderState.PushRenderTargetTexture(SSAO_BLUR_TARGET, ShaderStage::PixelShader);
			renderState.PushRenderTargetTexture(DEPTH_TARGET, ShaderStage::PixelShader);
			mSSAOBlur.GetStateParameters().SetVariable<Vector2f>("direction", Vector2f(0.0f, 1.0f));
			mSSAOBlur.Render(renderState);
			renderState.PopState();
		}

		{
			std::vector<Auto<RenderTarget>> renderStageTargets = {
				mRenderTargetDatabase.GetRenderTarget(COMBINED_TARGET),
			};

			mRenderTargetDatabase.SetCurrentTargetSize(renderTargetConfig.GetSize());
			graphics.SetRenderTargets(renderStageTargets, NULL);
			renderState.SetViewport(sceneView.CalculateViewport(renderTargetConfig.GetSize()), sceneView.GetDepthRange());

			renderState.PushState();
			renderState.PushRenderTargetTexture(SSAO_TARGET, ShaderStage::PixelShader);
			renderState.PushRenderTargetTexture(NORMAL_TARGET, ShaderStage::PixelShader);
			renderState.PushRenderTargetTexture(COLOR_TARGET, ShaderStage::PixelShader);
			mAmbientLighting.Render(renderState);
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

