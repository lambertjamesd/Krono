#pragma once

#include "OpenGLCommon.h"

#include "Interface/IndexBuffer.h"

namespace krono
{

class OpenGLIndexBuffer
{
public:
	static GLenum GetGLFormat(IndexBuffer::Format format);
private:
	static GLenum gFormatMapping[IndexBuffer::FormatCount];
};

}
