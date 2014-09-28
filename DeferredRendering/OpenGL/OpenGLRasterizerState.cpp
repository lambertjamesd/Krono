
#include "OpenGLRasterizerState.h"


namespace krono
{

OpenGLRasterizerState::OpenGLRasterizerState(const RasterizerStateDescription& description) :
	RasterizerState(description)
{

}

OpenGLRasterizerState::~OpenGLRasterizerState(void)
{

}

void OpenGLRasterizerState::Use()
{
	glPolygonMode(GL_FRONT_AND_BACK, gFillMode[mDescription.fillMode]);

	if (mDescription.cullMode == CullMode::None)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(gCullMode[mDescription.cullMode]);
	}

	glFrontFace(mDescription.frontCCW ? GL_CCW : GL_CW);
	glPolygonOffset(mDescription.slopeScaledDepthBias, (GLfloat)mDescription.depthBias);

	if (mDescription.depthClipEnable)
	{
		glDisable(GL_DEPTH_CLAMP);
	}
	else
	{
		glEnable(GL_DEPTH_CLAMP);
	}

	if (mDescription.scissorEnable)
	{
		glEnable(GL_SCISSOR_TEST);
	}
	else
	{
		glDisable(GL_SCISSOR_TEST);
	}

	if (mDescription.multisampleEnable)
	{
		glEnable(GL_MULTISAMPLE);
	}
	else
	{
		glDisable(GL_MULTISAMPLE);
	}

	if (mDescription.antialiasedLineEnable)
	{
		glEnable(GL_LINE_SMOOTH);
	}
	else
	{
		glDisable(GL_LINE_SMOOTH);
	}
}

GLenum OpenGLRasterizerState::gFillMode[FillMode::Count] = {
		GL_LINE,
		GL_FILL
};

GLenum OpenGLRasterizerState::gCullMode[CullMode::Count] = {
		0,
		GL_FRONT,
		GL_BACK
};

}
