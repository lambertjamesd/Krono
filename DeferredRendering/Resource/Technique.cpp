
#include "Technique.h"
#include "Compositor/RenderState.h"

namespace krono
{

Technique::Technique()
{

}

Technique::Technique(Auto<VertexShader>& vertexShader, Auto<PixelShader>& pixelShader)
{
	mRenderStateParameters.SetVertexShader(vertexShader);
	mRenderStateParameters.SetPixelShader(pixelShader);
}

Technique::~Technique(void)
{
}

void Technique::Use(RenderState& renderState)
{
	renderState.PushParameters(mRenderStateParameters);
}

RenderStateParameters& Technique::GetRenderStateParameters()
{
	return mRenderStateParameters;
}

}
