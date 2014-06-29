#pragma once

#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include "Shader.h"

class VertexBuffer;

class ShaderProgram
{
public:
	virtual ~ShaderProgram(void);

	virtual void Use() = 0;
	virtual void BindVertexBuffer(VertexBuffer& vertexBuffer) = 0;
protected:
	ShaderProgram();
private:
	ShaderProgram(const ShaderProgram& other);
	ShaderProgram& operator=(const ShaderProgram& other);
};