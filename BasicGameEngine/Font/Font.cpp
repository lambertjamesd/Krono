#include "Font.h"

using namespace krono;

namespace kge
{

CharacterInfo::CharacterInfo()
{

}

CharacterInfo::CharacterInfo(UnicodeCodepoint codepoint, float leftMargin, float width, float rightMargin) :
	codepoint(codepoint),
	leftMargin(leftMargin),
	width(width),
	rightMargin(rightMargin)
{

}

CharacterKerning::CharacterKerning(UnicodeCodepoint a, UnicodeCodepoint b, float kerning) :
	a(a),
	b(b),
	kerning(kerning)
{

}

Font::Font(const std::vector<CharacterInfo>& characterInfo, const std::vector<CharacterKerning>& kerning, const Texture2D::Ptr& texture) :
	mFontTexture(texture)
{
	for (auto it = characterInfo.begin(); it != characterInfo.end(); ++it)
	{
		mCharacterInfo[it->codepoint] = *it;
	}

	for (auto it = kerning.begin(); it != kerning.end(); ++it)
	{
		mKerningSpacing[(long long)it->a << 32 | (long long)it->b] = it->kerning;
	}
}


Font::~Font(void)
{
}

krono::Vector2f Font::GenerateGlyphs(const char* start, const char* end, const krono::Vector2f& position, const krono::ColorU8& color, Font::CharacterVertex* output, size_t maxVertexCount, float extraSpacing) const
{
	const char* currentCharacter = start;
	Font::CharacterVertex* bufferEnd = output + maxVertexCount;

	krono::Vector2f currentPosition = position;

	UnicodeCodepoint lastCharacter = 0;
	float previousRightMargin = 0.0f;

	while (*currentCharacter && output + 3 < bufferEnd)
	{
		UnicodeCodepoint codepoint;
		currentCharacter = ReadUT8Character(currentCharacter, codepoint, mDefaultCharacter);

		auto glyphFind = mCharacterInfo.find(codepoint);
		const CharacterInfo& characterInfo = (glyphFind == mCharacterInfo.end()) ? mCharacterInfo.find(mDefaultCharacter)->second : glyphFind->second;

		currentPosition.x += previousRightMargin + characterInfo.leftMargin + GetKerning(lastCharacter, codepoint);

		if (output != NULL)
		{
			output = WriteGlyph(output, currentPosition, color, characterInfo);
		}

		currentPosition.x += characterInfo.width + extraSpacing;

		previousRightMargin = characterInfo.rightMargin;
		lastCharacter = codepoint;
	}

	return currentPosition;
}

float Font::GetKerning(UnicodeCodepoint a, UnicodeCodepoint b) const
{
	auto findResult = mKerningSpacing.find((unsigned long long)a << 32 | (unsigned long long)b);

	if (findResult == mKerningSpacing.end())
	{
		return 0.0f;
	}
	else
	{
		return findResult->second;
	}
}

void Font::SetKerning(UnicodeCodepoint a, UnicodeCodepoint b, float value)
{
	mKerningSpacing[(unsigned long long)a << 32 | (unsigned long long)b] = value;
}

const krono::Texture2D::Ptr& Font::GetTexture() const
{
	return mFontTexture;
}

Font::CharacterVertex* Font::WriteGlyph(Font::CharacterVertex* output, const krono::Vector2f& currentPosition, const krono::ColorU8& color, const CharacterInfo& characterInfo) const
{
	output->mPosition = Vector3f(currentPosition + Vector2f(0.0f, mHeight), 0.0f);
	output->mTexCoord = characterInfo.textureOrigin + Vector2f(0.0f, characterInfo.textureSize.y);
	output->mColor = color;
	++output;
	
	output->mPosition = Vector3f(currentPosition + Vector2f(0.0f, 0.0f), 0.0f);
	output->mTexCoord = characterInfo.textureOrigin;
	output->mColor = color;
	++output;
	
	output->mPosition = Vector3f(currentPosition + Vector2f(characterInfo.width, 0.0f), 0.0f);
	output->mTexCoord = characterInfo.textureOrigin + Vector2f(characterInfo.textureSize.x, 0.0f);
	output->mColor = color;
	++output;
	
	output->mPosition = Vector3f(currentPosition + Vector2f(characterInfo.width, mHeight), 0.0f);
	output->mTexCoord = characterInfo.textureOrigin + Vector2f(characterInfo.textureSize.x, characterInfo.textureSize.y);
	output->mColor = color;
	++output;

	return output;
}

const char* Font::ReadUT8Character(const char* input, UnicodeCodepoint& codepointOut, UnicodeCodepoint defaultCharacter)
{
	char byteShift = *input;
	int byteCount = 0;

	while ((byteShift & 0x80) != 0)
	{
		byteShift <<= 1;
		++byteCount;
	}
	
	if (byteCount == 0)
	{
		codepointOut = *input;
		return input + 1;
	}
	else
	{
		codepointOut = byteShift >> byteCount;
		++input;

		for (int i = 1; i < byteCount; ++i)
		{
			if ((*input & 0xC) == 0xC)
			{
				codepointOut = (codepointOut << 6) | (*input & 0x3f);
				++input;
			}
			else
			{
				// invalid utf-8 encoding
				codepointOut = defaultCharacter;
				return input;
			}
		}

		return input;
	}
}

}