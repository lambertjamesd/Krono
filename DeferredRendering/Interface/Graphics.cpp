
#include "Graphics.h"

#ifdef USE_DX11
#include "DX11/DX11Graphics.h"
#pragma message("Compiling With DirectX 11")
#endif

#ifdef USE_OPENGL4_4
#include "OpenGL/OpenGLGraphics.h"
#pragma message("Compiling With OpenGL 4.4 11")
#endif

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

#ifdef USE_DX11
	case Graphics::DirectX11:
		return Auto<Graphics>(new DX11Graphics());
#endif

#ifdef USE_OPENGL4_4
	case Graphics::OpenGL:
		return Auto<Graphics>(new OpenGLGraphics());
#endif

	default:
		throw InvalidEnumException("Invalid Graphics API");
	}
}