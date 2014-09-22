#pragma once
#include <string>

#include "Core/Object.h"
#include "Core/Memory.h"
#include "ShaderInputLayout.h"

namespace krono
{

namespace ShaderStage
{
	enum Type
	{
		VertexShader,
		HullShader,
		DomainShader,
		GeometryShader,
		PixelShader,
		ComputeShader,
		TypeCount
	};
}

class Shader : public Object
{
public:
	typedef Auto<Shader> Ptr;

	void SetInputLayout(const ShaderInputLayout& inputLayout);
	const ShaderInputLayout& GetInputLayout();
	bool HasInputLayout() const;

protected:
	Shader(void);

	ShaderInputLayout mInputLayout;
};

class VertexShader : public Shader
{
public:
	typedef Auto<VertexShader> Ptr;

	virtual ~VertexShader(void);
	virtual bool IsValid() const = 0;
protected:
	VertexShader(void);
private:
};

class PixelShader : public Shader
{
public:
	typedef Auto<PixelShader> Ptr;

	virtual ~PixelShader(void);
	virtual bool IsValid() const = 0;
protected:
	PixelShader(void);
private:
};

}
