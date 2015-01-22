#pragma once

#include <Krono.h>
#include <memory>
#include "Font.h"

namespace kge
{

class FontGenerator
{
public:
	~FontGenerator(void);

	virtual void GetCharacterInfo(const char* characters, std::vector<CharacterInfo>& output) = 0;
	virtual void GetKerning(std::vector<CharacterKerning>& output) = 0;
	virtual void DrawUTF8Character(const CharacterInfo& character, const krono::Vector2i& position, int fontHeight) = 0;

	virtual void SetImageSize(size_t width, size_t height) = 0;
	virtual void CopyImageData(void* buffer) = 0;

	virtual FontInfo GetFontInfo() = 0;

	static std::unique_ptr<FontGenerator> CreateFontGenerator(const std::string& fontName, size_t pixelHeight);

protected:
	FontGenerator(void);
};

}