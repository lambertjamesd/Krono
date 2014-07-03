#pragma once
#include "..\Interface\Shader.h"
#include <vector>

class DX11VertexShader : public VertexShader
{
public:
	DX11VertexShader(ID3D11Device* device, const std::vector<char>& compiledData);
	~DX11VertexShader(void);
private:
	ID3D11VertexShader *mVertexShader;
};

class DX11FragmentShader : public FragmentShader
{
public:
	DX11FragmentShader(void);
	~DX11FragmentShader(void);
private:
	ID3D11FragmentShader *mFragmentShader;
};

