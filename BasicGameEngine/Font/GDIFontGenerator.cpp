#include "GDIFontGenerator.h"
#include <Krono.h>

using namespace std;

namespace kge
{

GDIFontGenerator::GDIFontGenerator(const std::string& fontName, size_t pixelHeight) :
	mDrawBitmap(NULL),
	mPreviousBitmap(NULL),
	mLastFont(NULL)
{
	mHDC = CreateCompatibleDC(NULL);

	mFont = CreateFont(pixelHeight, 0, 0, 0, FW_DONTCARE, 
		false, false, false, ANSI_CHARSET,
		OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, 
		ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Helvetica"));

	mLastFont = (HFONT)SelectObject(mHDC, mFont);

	SetTextColor(mHDC, RGB(255, 255, 255));
	SetBkColor(mHDC, RGB(0, 0, 0));
}


GDIFontGenerator::~GDIFontGenerator(void)
{
	CleanupBitmap();
	SelectObject(mHDC, mLastFont);
	DeleteObject(mFont);
	DeleteDC(mHDC);
}

void GDIFontGenerator::GetCharacterInfo(const char* characters, std::vector<CharacterInfo>& output)
{
	while (*characters)
	{
		UnicodeCodepoint currentCharacter;
		characters = Font::ReadUT8Character(characters, currentCharacter, 0);

		ABCFLOAT characterInfo;
		GetCharABCWidthsFloat(mHDC, currentCharacter, currentCharacter, &characterInfo);

		output.push_back(CharacterInfo(currentCharacter, characterInfo.abcfA, characterInfo.abcfB, characterInfo.abcfC));
	}
}

void GDIFontGenerator::GetKerning(std::vector<CharacterKerning>& output)
{
	size_t kerningPairCount = GetKerningPairs(mHDC, 0, NULL);
	std::vector<KERNINGPAIR> kerningPairs(kerningPairCount);
	GetKerningPairs(mHDC, kerningPairCount, &kerningPairs.front());

	output.reserve(kerningPairCount);
	for (auto it = kerningPairs.begin(); it != kerningPairs.end(); ++it)
	{
		output.push_back(CharacterKerning(it->wFirst, it->wSecond, (float)it->iKernAmount));
	}
}

void GDIFontGenerator::DrawUTF8Character(const CharacterInfo& character, const krono::Vector2i& position, int fontHeight)
{
	TCHAR tChar = (TCHAR)character.codepoint;
	RECT rect;
	rect.left = position.x;
	rect.top = position.y;
	rect.right = position.x + (int)character.width;
	rect.bottom = position.y + fontHeight;
	ExtTextOut(mHDC, position.x - (int)ceil(character.leftMargin), position.y, ETO_CLIPPED, &rect, &tChar, 1, NULL);
	//DrawText(mHDC, &tChar, 1, &rect, DT_LEFT | DT_TOP | DT_NOCLIP);
}

void GDIFontGenerator::SetImageSize(size_t width, size_t height)
{
	CleanupBitmap();

	mDrawBitmap = CreateBitmap(width, height, 1, 32, NULL);
	mPreviousBitmap = (HBITMAP)SelectObject(mHDC, mDrawBitmap);
	
	HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.bottom = height;
	rect.right = width;
	FillRect(mHDC, &rect, blackBrush);
	DeleteObject(blackBrush);

	mImageWidth = width;
	mImageHeight = height;
}

void GDIFontGenerator::CopyImageData(void* buffer)
{
	std::vector<long> lpPixels(mImageWidth * mImageHeight);
	GetBitmapBits(mDrawBitmap, lpPixels.size() * sizeof(long), &lpPixels.front());

	BYTE* output = (BYTE*)buffer;

	// convert output to greyscale
	for (size_t i = 0; i < mImageWidth * mImageHeight; ++i)
	{
		output[i] = GetRValue(lpPixels[i]);
	}
}

FontInfo GDIFontGenerator::GetFontInfo()
{
	FontInfo result;

	TEXTMETRIC metrics;
	GetTextMetrics(mHDC, &metrics);

	result.height = metrics.tmHeight;
	result.ascent = metrics.tmAscent;

	return result;
}

void GDIFontGenerator::CleanupBitmap()
{
	if (mDrawBitmap)
	{
		SelectObject(mHDC, mPreviousBitmap);
		DeleteObject(mDrawBitmap);
		mDrawBitmap = NULL;
	}
}

}