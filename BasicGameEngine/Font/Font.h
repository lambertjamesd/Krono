#pragma once

#include <Krono.h>
#include <unordered_map>
#include <string>

namespace kge
{

typedef unsigned long UnicodeCodepoint;

class Font : public krono::Resource
{
public:
	Font(void);
	~Font(void);

	struct CharacterVertex
	{
		krono::Vector3f mPosition;
		krono::Vector2f mTexCoord;
	};

	static size_t GlyphCount(const std::string& string);
	void GenerateGlyphs(const std::string& string, const krono::Vector2f& position, float maxWidth);

	static const char* ReadUT8Character(const char* input, UnicodeCodepoint& codepointOut);
private:
	struct CharacterPosition
	{
		krono::Vector2i mOrigin;
		krono::Vector2i mSize;
	};

	std::unordered_map<UnicodeCodepoint, float> mCharacterSpacing;
	std::unordered_map<unsigned long long, CharacterPosition> mCharacterPosition;
	krono::Texture2D::Ptr mFontTexture;

	size_t mHeight;
	size_t mBaseline;
};

}