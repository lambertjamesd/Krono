#include "Color.h"
#include <algorithm>

namespace krono
{
	Colorf ColorU82F(const ColorU8& byteColor)
	{
		return Colorf(byteColor.r / 255.0f, byteColor.g / 255.0f, byteColor.b / 255.0f, byteColor.a / 255.0f);
	}

	ColorU8 ColorF2U8(const Colorf& floatColor)
	{
		return ColorU8(
			(unsigned char)std::min(floatColor.r * 255.0f, 255.0f),
			(unsigned char)std::min(floatColor.g * 255.0f, 255.0f),
			(unsigned char)std::min(floatColor.b * 255.0f, 255.0f),
			(unsigned char)std::min(floatColor.a * 255.0f, 255.0f));
	}
}