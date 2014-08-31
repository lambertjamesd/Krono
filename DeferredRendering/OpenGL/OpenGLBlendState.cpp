#include "OpenGLBlendState.h"

namespace krono
{

OpenGLBlendState::OpenGLBlendState(const BlendStateDescription& description) :
	BlendState(description)
{
}


OpenGLBlendState::~OpenGLBlendState(void)
{
}

const GLenum OpenGLBlendState::gBlendMapping[Blend::Count] = {
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA_SATURATE,
	GL_CONSTANT_COLOR,
	GL_ONE_MINUS_CONSTANT_COLOR,
	GL_SRC1_COLOR,
	GL_ONE_MINUS_SRC_COLOR,

	GL_SRC1_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
};

const GLenum OpenGLBlendState::gOperatorMapping[BlendOperator::Count] = {
	GL_FUNC_ADD,
	GL_FUNC_SUBTRACT,
	GL_FUNC_REVERSE_SUBTRACT,
	GL_MIN,
	GL_MAX
};

void OpenGLBlendState::Use()
{
	for (size_t i = 0; i < MaxRenderTargetCount; ++i)
	{
		RenderTargetBlend& renderTarget = mDescription.renderTargets[i];

		if (renderTarget.blendEnabled)
		{
			glEnablei(GL_BLEND, i);
		}
		else
		{
			glDisablei(GL_BLEND, i);
		}

		glBlendFuncSeparatei(i, 
			gBlendMapping[renderTarget.srcBlend], 
			gBlendMapping[renderTarget.destBlend], 
			gBlendMapping[renderTarget.srcBlendAlpha], 
			gBlendMapping[renderTarget.destBlendAlpha]);

		glBlendEquationSeparatei(i, 
			gOperatorMapping[renderTarget.blendOperation],
			gOperatorMapping[renderTarget.blendOperationAlpha]);

		glColorMaski(i, 
			(renderTarget.blendMask | BlendMask::Red) != 0,
			(renderTarget.blendMask | BlendMask::Green) != 0,
			(renderTarget.blendMask | BlendMask::Blue) != 0,
			(renderTarget.blendMask | BlendMask::Alpha) != 0);
	}

	glBlendColor(mDescription.blendFactor.r, mDescription.blendFactor.g, mDescription.blendFactor.b, mDescription.blendFactor.a);
}

}