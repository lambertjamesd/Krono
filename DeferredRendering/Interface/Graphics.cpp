#include "stdafx.h"
#include "Graphics.h"
#include "..\DX11\DX11Graphics.h"
#include "..\OpenGL\OpenGLGraphics.h"
#include "InvalidEnumException.h"

Graphics::Graphics(void)
{
}


Graphics::~Graphics(void)
{
}


Auto<Graphics> Graphics::CreateGraphics(Graphics::API api)
{
	switch (api)
	{
	case Graphics::DirectX11:
		return Auto<Graphics>(new DX11Graphics());
	case Graphics::OpenGL:
		return Auto<Graphics>(new OpenGLGraphics());
	default:
		throw InvalidEnumException("Invalid Graphics API");
	}
}

GLenum Graphics::gGLTypeMapping[] = {GL_FLOAT, GL_UNSIGNED_BYTE};