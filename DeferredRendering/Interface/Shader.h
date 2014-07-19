#pragma once
#include <string>

class VertexShader
{
public:
	virtual ~VertexShader(void);
	virtual bool IsValid() const = 0;
protected:
	VertexShader(void);
private:
};

class PixelShader
{
public:
	virtual ~PixelShader(void);
	virtual bool IsValid() const = 0;
protected:
	PixelShader(void);
private:
};