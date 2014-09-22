
#include "Shader.h"
#include <vector>
#include <iostream>

namespace krono
{

void Shader::SetInputLayout(const ShaderInputLayout& inputLayout)
{
	mInputLayout = inputLayout;
}

const ShaderInputLayout& Shader::GetInputLayout()
{
	return mInputLayout;
}
	
Shader::Shader(void)
{

}

VertexShader::VertexShader(void)
{

}

VertexShader::~VertexShader(void)
{

}


PixelShader::PixelShader(void)
{

}

PixelShader::~PixelShader(void)
{

}

}
