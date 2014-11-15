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

std::unique_ptr<FontGenerator> FontGenerator::CreateFontGenerator()
{
#ifdef WIN32
	return move(unique_ptr<FontGenerator>());
#else
#error no font generator specified
#endif

}

}