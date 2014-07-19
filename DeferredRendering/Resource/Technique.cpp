#include "stdafx.h"
#include "Technique.h"


Technique::Technique(Auto<VertexShader>& vertexShader, Auto<PixelShader>& pixelShader) :
	mVertexShader(vertexShader),
	mPixelShader(pixelShader)
{

}

Technique::~Technique(void)
{
}

