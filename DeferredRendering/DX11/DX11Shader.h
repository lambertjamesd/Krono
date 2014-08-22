#pragma once

#include "DX11Common.h"

#include "Interface/Shader.h"
#include <string>
#include "DX11VertexBuffer.h"
#include <map>
#include "DX11SmartPtr.h"

namespace krono
{

class DX11VertexShader : public VertexShader
{
public:
	DX11VertexShader(ID3D11Device* device, const std::string& compiledData);
	virtual ~DX11VertexShader(void);
	virtual bool IsValid() const;
	
	ID3D11InputLayout* GetInputLayout(const InputLayout& inputLayout);

	void Use();
private:
	
	ID3D11InputLayout* CreateInputLayout(const InputLayout& inputLayout) const;

	std::string mBytecode;
	ID3D11VertexShader *mVertexShader;
	ID3D11Device *mDevice;
	
	std::map<UInt32, DX11SmartPtr<ID3D11InputLayout> > mInputLayouts;
};

class DX11PixelShader : public PixelShader
{
public:
	DX11PixelShader(ID3D11Device* device, const std::string& compiledData);
	virtual ~DX11PixelShader(void);
	virtual bool IsValid() const;
	
	void Use();
private:
	ID3D11PixelShader *mPixelShader;
	ID3D11Device *mDevice;
};

}

