#pragma once

#include <Krono.h>
#include <memory>

namespace kge
{

class FontGenerator
{
public:
	~FontGenerator(void);

	virtual float GetCharacterWidth(const char* utfCharacter) = 0;
	virtual float GetCharacterSpacing(const char* firstCharacter, const char* secondCharacter) = 0;
	virtual void DrawUTF8Character(const char* utfCharacter, const krono::Vector2i& position) = 0;

	static std::unique_ptr<FontGenerator> CreateFontGenerator();

protected:
	FontGenerator(void);
};

}