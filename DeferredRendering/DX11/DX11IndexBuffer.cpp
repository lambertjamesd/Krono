
#include "DX11IndexBuffer.h"
#include "HResultException.h"

namespace krono
{

DXGI_FORMAT DX11IndexBuffer::gFormatMapping[IndexBuffer::FormatCount] = {
	DXGI_FORMAT_R16_UINT,
	DXGI_FORMAT_R32_UINT
};

DXGI_FORMAT DX11IndexBuffer::GetDXFormat(IndexBuffer::Format format)
{
	return gFormatMapping[format];
}

}
