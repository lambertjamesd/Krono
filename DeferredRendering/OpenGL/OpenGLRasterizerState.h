#pragma once

#include "Interface/RasterizerState.h"
#include "OpenGLCommon.h"

namespace krono
{

class OpenGLRasterizerState : public RasterizerState
{
public:
	OpenGLRasterizerState(const RasterizerStateDescription& description);
	~OpenGLRasterizerState(void);

	void Use();
private:
	static GLenum gFillMode[FillMode::Count];
	static GLenum gCullMode[CullMode::Count];
};

}
