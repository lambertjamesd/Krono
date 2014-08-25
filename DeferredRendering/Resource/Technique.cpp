
#include "Technique.h"
#include "Compositor/RenderState.h"

namespace krono
{

Technique::Technique()
{

}

Technique::Technique(Auto<VertexShader>& vertexShader, Auto<PixelShader>& pixelShader) :
	mVertexShader(vertexShader),
	mPixelShader(pixelShader)
{

}

Technique::~Technique(void)
{
}

void Technique::Use(RenderState& renderState)
{
	if (mVertexShader != NULL)
	{
		renderState.SetVertexShader(mVertexShader);
	}

	if (mPixelShader != NULL)
	{
		renderState.SetPixelShader(mPixelShader);
	}
}

}
