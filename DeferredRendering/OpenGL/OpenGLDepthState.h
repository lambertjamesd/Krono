#pragma once

#include "Interface/DepthState.h"
#include "OpenGLCommon.h"

namespace krono
{

class OpenGLDepthState : public DepthState
{
public:
	OpenGLDepthState(const DepthStateDescription& description);
	~OpenGLDepthState(void);

	void Use(GLint reference) const;
	
	static GLenum GetComparisonFunction(CompareFunction::Type compareFunction);
private:
	static GLenum gComparisonFunction[CompareFunction::Count];
	static GLenum gStencilOperations[StencilOperation::Count];
};

}