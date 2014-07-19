#pragma once

#include "..\Interface\Shader.h"
#include "..\Core\Memory.h"

class Technique
{
public:
	Technique(Auto<VertexShader>& vertexShader, Auto<PixelShader>& pixelShader);
	~Technique(void);
private:
	Auto<VertexShader> mVertexShader;
	Auto<PixelShader> mPixelShader;
};

