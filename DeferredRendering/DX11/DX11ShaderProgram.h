#pragma once
#include "../Interface/ShaderProgram.h"
#include "DX11Shader.h"
#include "../Core/Memory.h"
#include <map>
#include "DX11SmartPtr.h"

class DX11ShaderProgram : public ShaderProgram
{
public:
	DX11ShaderProgram(ID3D11Device* device, const Auto<DX11VertexShader>& vertexShader, const Auto<DX11FragmentShader>& fragmentShader);
	~DX11ShaderProgram(void);
	
	virtual void Use();
	virtual void BindVertexBuffer(VertexBuffer& vertexBuffer);
private:
	DX11ShaderProgram(const DX11ShaderProgram& other);
	DX11ShaderProgram& operator=(const DX11ShaderProgram& other);

	Auto<DX11VertexShader> mVertexShader;
	Auto<DX11FragmentShader> mFragmentShader;

	std::map<UINT32, DX11SmartPtr<ID3D11InputLayout> > mInputLayouts;

	ID3D11Device *mDevice;
};

