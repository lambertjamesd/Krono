#include "Font.h"

namespace kge
{

Font::Font(void)
{
}


Font::~Font(void)
{
}

const char* Font::ReadUT8Character(const char* input, UnicodeCodepoint& codepointOut)
{
	char byteShift = *input;
	int byteCount = 0;

	while ((byteShift & 0x80) == 0)
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
				codepointOut = 0;
				return input;
			}
		}

		return input;
	}
}

}