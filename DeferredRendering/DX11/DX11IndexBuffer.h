#pragma once

#include "DX11Common.h"

#include "Interface/IndexBuffer.h"
#include "DX11SmartPtr.h"

namespace krono
{

class DX11IndexBuffer : public IndexBuffer
{
public:
	static DXGI_FORMAT GetDXFormat(IndexBuffer::Format format);
public:
	static DXGI_FORMAT gFormatMapping[IndexBuffer::FormatCount];
};

}
