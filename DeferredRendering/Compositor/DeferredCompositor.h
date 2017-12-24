#pragma once

#include "Compositor.h"
#include "RenderSceneCompositeStage.h"
#include "LightingCompositeStage.h"
#include "ScreenQuadCompositeStage.h"
#include "../Resource/ResourceManager.h"

namespace krono
{
	class DeferredCompositor : public Compositor
	{
	public:
		DeferredCompositor(Graphics& graphics, ResourceManager& resourceManager);
		~DeferredCompositor();

		virtual void Render(Graphics& graphics, const RenderTargetConfiguration& renderTargetConfig, SceneView& sceneView);
	private:
		enum {
			COLOR_TARGET,
			NORMAL_TARGET,
			SPECULAR_TARGET,
			SSAO_TARGET,
			SSAO_BLUR_TARGET,
			COMBINED_TARGET,

			DEPTH_TARGET,
		};

		RenderSceneCompositeStage mRenderScene;
		LightingCompositeStage mRenderLighting;
		ScreenQuadCompositeStage mToneMapper;

		ScreenQuadCompositeStage mSSAO;
		ScreenQuadCompositeStage mSSAOBlur;
		ScreenQuadCompositeStage mAmbientLighting;
	};
}

