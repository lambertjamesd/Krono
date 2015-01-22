#pragma once

#include <Krono.h>
#include "Font.h"
#include "FontGenerator.h"

namespace kge
{
	
class FontLoader : public krono::ResourceLoader
{
public:
	FontLoader(void);
	~FontLoader(void);
	
	virtual Auto<krono::Resource> LoadResource(krono::ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);
private:
	static const char* gAnsiCharset;

	static float WidthSum(const std::vector<CharacterInfo>& character);

	static bool LayoutText(FontGenerator* output, std::vector<CharacterInfo>& character, int fontHeight, int width, int height);
};

}