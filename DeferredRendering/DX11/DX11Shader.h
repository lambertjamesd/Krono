#pragma once
#include "..\Interface\Shader.h"
#include <string>
#include "DX11VertexBuffer.h"

class DX11VertexShader : public VertexShader
{
public:
	DX11VertexShader(ID3D11Device* device, const std::string& compiledData);
	virtual ~DX11VertexShader(void);
	virtual bool IsValid() const;
	
	ID3D11InputLayout* CreateInputLayout(const InputLayout& inputLayout) const;

	void Use();
private:
	static DXGI_FORMAT gFormatMapping[][4];

	std::string mBytecode;
	ID3D11VertexShader *mVertexShader;
	ID3D11Device *mDevice;
};

class DX11FragmentShader : public FragmentShader
{
public:
	DX11FragmentShader(ID3D11Device* device, const std::string& compiledData);
	virtual ~DX11FragmentShader(void);
	virtual bool IsValid() const;
	
	void Use();
private:
	ID3D11PixelShader *mPixelShader;
	ID3D11Device *mDevice;
};

