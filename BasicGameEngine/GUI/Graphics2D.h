#pragma once

#include "Font/Font.h"
#include "Transform2D.h"
#include "SpriteBatch2D.h"
#include "Font/Font.h"
#include <vector>
#include <string>
#include <memory>

namespace kge
{

class Graphics2D
{
public:
	Graphics2D(const krono::Graphics::Ptr& graphics);
	~Graphics2D(void);

	void DrawImage(const krono::Texture::Ptr& texture, const krono::Vector2f& position, const krono::Vector2f& size, const krono::Vector2f& spriteOrigin, const krono::Vector2f& spriteSize);
	void DrawString(const Font::Ptr& font, const krono::Vector2f& position, const std::string& utf8String);

	void SetTransform(const Transform2D& value);
	void ConcatTransform(const Transform2D& value);
	void ConcatTransform(const krono::Vector2f& position, const krono::Vector2f& right, const krono::Vector2f& scale);
	const Transform2D& GetCurrentTransform() const;
private:
	const static int SPRITE_BATCH_COUNT = 1;

	Transform2D mCurrentTransform;
	SpriteBatch2D::Ptr mActiveSpriteBatches[SPRITE_BATCH_COUNT];
	krono::Texture2D::Ptr mCurrentTexture;
	krono::PixelShader::Ptr mCurrentShader;

	krono::PixelShader::Ptr mDefaultShader;
	krono::VertexShader::Ptr mVertexShader;

	krono::Graphics::Ptr mGraphics;
};

}
