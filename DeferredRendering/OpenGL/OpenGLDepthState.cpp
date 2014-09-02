#include "OpenGLDepthState.h"

namespace krono
{

OpenGLDepthState::OpenGLDepthState(const DepthStateDescription& description) :
	DepthState(description)
{

}


OpenGLDepthState::~OpenGLDepthState(void)
{

}

void OpenGLDepthState::Use(GLint reference) const
{
	if (mDescription.depthEnable)
	{
		glEnable(GL_DEPTH);
		glDepthMask(mDescription.depthWriteEnabled);
		glDepthFunc(gComparisonFunction[mDescription.depthCompare]);
	}
	else
	{
		glDisable(GL_DEPTH);
	}

	if (mDescription.stencilEnable)
	{
		glEnable(GL_STENCIL_TEST);

		glStencilMask(mDescription.stencilWriteMask);

		glStencilOpSeparate(GL_FRONT, 
			gStencilOperations[mDescription.frontFace.stencilFail],
			gStencilOperations[mDescription.frontFace.stencilDepthFail],
			gStencilOperations[mDescription.frontFace.stencilPass]);
		glStencilFuncSeparate(GL_FRONT,
			gComparisonFunction[mDescription.frontFace.stencilFunction],
			reference,
			mDescription.stencilReadMask);
		
		glStencilOpSeparate(GL_BACK, 
			gStencilOperations[mDescription.backFace.stencilFail],
			gStencilOperations[mDescription.backFace.stencilDepthFail],
			gStencilOperations[mDescription.backFace.stencilPass]);
		glStencilFuncSeparate(GL_BACK,
			gComparisonFunction[mDescription.backFace.stencilFunction],
			reference,
			mDescription.stencilReadMask);
	}
	else
	{
		glDisable(GL_STENCIL_TEST);
	}
}

GLenum OpenGLDepthState::GetComparisonFunction(CompareFunction::Type compareFunction)
{
	return gComparisonFunction[compareFunction];
}

GLenum OpenGLDepthState::gComparisonFunction[CompareFunction::Count] = {
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS
};

GLenum OpenGLDepthState::gStencilOperations[StencilOperation::Count] = {
	GL_KEEP,
	GL_ZERO,
	GL_REPLACE,
	GL_INCR,
	GL_DECR,
	GL_INVERT,
	GL_INCR_WRAP,
	GL_DECR_WRAP
};

}