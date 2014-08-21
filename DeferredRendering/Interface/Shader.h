#pragma once
#include <string>

#include "Core/Object.h"

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

class VertexShader : public Object
{
public:
	virtual ~VertexShader(void);
	virtual bool IsValid() const = 0;
protected:
	VertexShader(void);
private:
};

class PixelShader : public Object
{
public:
	virtual ~PixelShader(void);
	virtual bool IsValid() const = 0;
protected:
	PixelShader(void);
private:
};