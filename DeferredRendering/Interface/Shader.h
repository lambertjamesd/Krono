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

class FragmentShader
{
public:
	virtual ~FragmentShader(void);
	virtual bool IsValid() const = 0;
protected:
	FragmentShader(void);
private:
};