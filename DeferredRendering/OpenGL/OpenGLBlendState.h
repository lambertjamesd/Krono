#pragma once

#include "Interface/BlendState.h"
#include "OpenGLCommon.h"

namespace krono
{

class OpenGLBlendState : public BlendState
{
public:
	OpenGLBlendState(const BlendStateDescription& description);
	~OpenGLBlendState(void);

	void Use();
private:
	static const GLenum gBlendMapping[Blend::Count];
	static const GLenum gOperatorMapping[BlendOperator::Count];
};

}