
#include "OpenGLIndexBuffer.h"

namespace krono
{

GLenum OpenGLIndexBuffer::gFormatMapping[IndexBuffer::FormatCount] = {
	GL_UNSIGNED_SHORT,
	GL_UNSIGNED_INT
};

GLenum OpenGLIndexBuffer::GetGLFormat(IndexBuffer::Format format)
{
	return gFormatMapping[format];
}

}
