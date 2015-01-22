#pragma once

#include "FontGenerator.h"
#include <Windows.h>

namespace kge
{

class GDIFontGenerator : public FontGenerator
{
public:
	GDIFontGenerator(const std::string& fontName, size_t pixelHeight);
	~GDIFontGenerator(void);
	
	virtual void GetCharacterInfo(const char* characters, std::vector<CharacterInfo>& output);
	virtual void GetKerning(std::vector<CharacterKerning>& output);
	virtual void DrawUTF8Character(const CharacterInfo& character, const krono::Vector2i& position, int fontHeight);
	
	virtual void SetImageSize(size_t width, size_t height);
	virtual void CopyImageData(void* buffer);
	
	virtual FontInfo GetFontInfo();

private:

	void CleanupBitmap();

	HDC mHDC;
	HBITMAP mDrawBitmap;
	HBITMAP mPreviousBitmap;

	HFONT mFont;
	HFONT mLastFont;

	size_t mImageWidth;
	size_t mImageHeight;
};

}