#pragma once

#include <Krono.h>
#include <unordered_map>
#include <string>

namespace kge
{

typedef unsigned long UnicodeCodepoint;

struct CharacterInfo
{
	CharacterInfo();
	CharacterInfo(UnicodeCodepoint codepoint, float leftMargin, float width, float rightMargin);

	UnicodeCodepoint codepoint;

	krono::Vector2f textureOrigin;
	krono::Vector2f textureSize;
		
	float leftMargin;
	float width;
	float rightMargin;
};

struct CharacterKerning
{
	CharacterKerning(UnicodeCodepoint a, UnicodeCodepoint b, float kerning);

	UnicodeCodepoint a;
	UnicodeCodepoint b;
	float kerning;
};

struct FontInfo
{
	int height;
	int ascent;
};

class Font : public krono::Resource
{
public:
	typedef Auto<Font> Ptr;

	Font(const std::vector<CharacterInfo>& characterInfo, const std::vector<CharacterKerning>& kerning, const krono::Texture2D::Ptr& texture);
	~Font(void);

	struct CharacterVertex
	{
		krono::Vector3f mPosition;
		krono::Vector2f mTexCoord;
		krono::ColorU8 mColor;
	};

	static size_t GlyphCount(const char* start, const char* end);
	krono::Vector2f GenerateGlyphs(const char* start, const char* end, const krono::Vector2f& position, const krono::ColorU8& color, CharacterVertex* ouput, size_t maxVertexCount, float extraSpacing = 0.0f) const;
	float GetKerning(UnicodeCodepoint a, UnicodeCodepoint b) const;
	void SetKerning(UnicodeCodepoint a, UnicodeCodepoint b, float value);

	const krono::Texture2D::Ptr& GetTexture() const;
	
	static const char* ReadUT8Character(const char* input, UnicodeCodepoint& codepointOut, UnicodeCodepoint defaultCharacter = 0);
private:
	CharacterVertex* WriteGlyph(CharacterVertex* output, const krono::Vector2f& currentPosition, const krono::ColorU8& color, const CharacterInfo& characterInfo) const;

	std::unordered_map<long long, float> mKerningSpacing;
	std::unordered_map<UnicodeCodepoint, CharacterInfo> mCharacterInfo;
	krono::Texture2D::Ptr mFontTexture;

	UnicodeCodepoint mDefaultCharacter;

	float mHeight;
	float mAscent;
};

}