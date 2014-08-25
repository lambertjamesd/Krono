#pragma once
#include <string>

#include "Core/Object.h"
#include "Core/Memory.h"

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

class VertexShader : public Object
{
public:
	typedef Auto<VertexShader> Ptr;

	virtual ~VertexShader(void);
	virtual bool IsValid() const = 0;
protected:
	VertexShader(void);
private:
};

class PixelShader : public Object
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
