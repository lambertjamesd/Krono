#pragma once

#include "Interface/Shader.h"
#include "Core/Memory.h"
#include "Interface/Graphics.h"

namespace krono
{

class Technique
{
public:
	Technique();
	Technique(Auto<VertexShader>& vertexShader, Auto<PixelShader>& pixelShader);
	~Technique(void);

	void Use(Graphics& graphics);
private:
	Auto<VertexShader> mVertexShader;
	Auto<PixelShader> mPixelShader;
};

}
