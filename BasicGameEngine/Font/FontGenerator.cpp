#include "FontGenerator.h"

#ifdef WIN32
#include "GDIFontGenerator.h"
#endif

using namespace std;

namespace kge
{

FontGenerator::FontGenerator(void)
{
}


FontGenerator::~FontGenerator(void)
{
}

std::unique_ptr<FontGenerator> FontGenerator::CreateFontGenerator(const std::string& fontName, size_t pixelHeight)
{
#ifdef WIN32
	return move(unique_ptr<FontGenerator>(new GDIFontGenerator(fontName, pixelHeight)));
#else
#error no font generator specified
#endif

}

}