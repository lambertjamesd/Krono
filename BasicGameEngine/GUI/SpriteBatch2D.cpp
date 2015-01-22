#include "SpriteBatch2D.h"

using namespace krono;

namespace kge
{

SpriteBatch2D::SpriteBatch2D(const krono::Graphics::Ptr& graphics, size_t maxQuadCount) :
	mMaxQuadCount(maxQuadCount)
{
	mIndexBuffer = graphics->CreateIndexBuffer(IndexBuffer::UInt16, BufferAccess::Dynamic);

	InputLayout inputLayout;
	inputLayout.AddAttribute(Attribute("POSITION", DataFormat(DataFormat::Float, 3)));
	inputLayout.AddAttribute(Attribute("TEXCOORD", DataFormat(DataFormat::Float, 2)));
	inputLayout.AddAttribute(Attribute("COLOR", DataFormat(DataFormat::Int8UNorm, 4)));

	mVertexBuffer = graphics->CreateVertexBuffer(inputLayout, BufferAccess::Dynamic);
}


SpriteBatch2D::~SpriteBatch2D(void)
{
}

}