#include "stdafx.h"
#include "DX11ShaderProgram.h"


DX11ShaderProgram::DX11ShaderProgram(ID3D11Device* device, const Auto<DX11VertexShader>& vertexShader, const Auto<DX11FragmentShader>& fragmentShader) :
	mDevice(device),
	mVertexShader(vertexShader),
	mFragmentShader(fragmentShader)
{

}


DX11ShaderProgram::~DX11ShaderProgram(void)
{

}

void DX11ShaderProgram::Use()
{
	mVertexShader->Use();
	mFragmentShader->Use();
}

void DX11ShaderProgram::BindVertexBuffer(VertexBuffer& vertexBuffer)
{
	UINT32 formatSignature = vertexBuffer.GetInputLayout().GetSignature();

	ID3D11InputLayout *inputLayout = NULL;
	auto existingLayout = mInputLayouts.find(formatSignature);

	if (existingLayout == mInputLayouts.end())
	{
		inputLayout = mVertexShader->CreateInputLayout(vertexBuffer.GetInputLayout());
		mInputLayouts[formatSignature] = inputLayout;
		inputLayout->Release();
	}
	else
	{
		inputLayout = existingLayout->second.Get();
	}

	ID3D11DeviceContext *context;
	mDevice->GetImmediateContext(&context);
	context->IASetInputLayout(inputLayout);
}