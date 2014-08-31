#pragma once

#include "Interface/Shader.h"
#include "Core/Memory.h"
#include "Interface/Graphics.h"
#include "Compositor/RenderStateParameters.h"

namespace krono
{
	
class RenderState;

class Technique
{
public:
	Technique();
	Technique(Auto<VertexShader>& vertexShader, Auto<PixelShader>& pixelShader);
	~Technique(void);

	void Use(RenderState& renderState);
	RenderStateParameters& GetRenderStateParameters();
private:
	RenderStateParameters mRenderStateParameters;
};

}
