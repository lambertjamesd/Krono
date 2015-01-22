#pragma once

#include <Krono.h>
#include <memory>

namespace kge
{

struct SpriteVertex
{
	krono::Vector3f mPosition;
	krono::Vector2f mTexCoord;
	krono::ColorU8 mColor;
};

class SpriteBatch2D
{
public:
	typedef std::unique_ptr<SpriteBatch2D> Ptr;

	SpriteBatch2D(const krono::Graphics::Ptr& graphics, size_t maxQuadCount);
	~SpriteBatch2D(void);

	void SetTexture(const krono::Texture::Ptr& texture, size_t slot);
	void SetShader(const krono::PixelShader::Ptr& shader);
	void AppendQuad(const SpriteVertex* vertices);

	void DrawBatch();
private:
	size_t mMaxQuadCount;

	krono::Texture::Ptr mTexture;
	krono::PixelShader::Ptr mPixelShader;

	krono::IndexBuffer::Ptr mIndexBuffer;
	krono::VertexBuffer::Ptr mVertexBuffer;

	SpriteVertex* mCurrentVertex;
	SpriteVertex* mBufferEnd;
};

}