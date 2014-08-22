
#include "Technique.h"


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

void Technique::Use(Graphics& graphics)
{
	graphics.SetVertexShader(mVertexShader);
	graphics.SetPixelShader(mPixelShader);
}

}
