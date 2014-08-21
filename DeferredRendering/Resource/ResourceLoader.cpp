
#include "ResourceLoader.h"

namespace krono
{

ResourceLoader::ResourceLoader(void)
{
}


ResourceLoader::~ResourceLoader(void)
{
}

std::string ResourceLoader::StringFromIStream(std::istream& inputStream)
{
	std::string contents;
	inputStream.seekg(0, std::ios::end);
	contents.resize((unsigned int)inputStream.tellg());
	inputStream.seekg(0, std::ios::beg);
	inputStream.read(&contents[0], contents.size());

	return contents;
}

}
