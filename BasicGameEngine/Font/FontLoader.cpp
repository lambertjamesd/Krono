#include "FontLoader.h"
#include "FontGenerator.h"

using namespace krono;
using namespace std;

namespace kge
{

const char* FontLoader::gAnsiCharset = 
	" !\"#$%&'()*+,-./0123456789"
	":;<=>?@"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"[\\]^_`"
	"abcdefghijklmnopqrstuvwxyz"
	"{|}~";

FontLoader::FontLoader(void)
{
}


FontLoader::~FontLoader(void)
{
}

Auto<krono::Resource> FontLoader::LoadResource(krono::ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName)
{
	size_t firstSpace = internalName.find(" ");

	if (firstSpace == std::string::npos)
	{
		throw FormatException("Font must be loaded using the following format #12px Helvetica");
	}

	std::string fontSizeText = internalName.substr(0, firstSpace);
	std::string fontFace = internalName.substr(firstSpace + 1);

	std::unique_ptr<FontGenerator> generator(move(FontGenerator::CreateFontGenerator(fontFace, atoi(fontSizeText.c_str()))));

	FontInfo fontInfo = generator->GetFontInfo();
	std::vector<CharacterInfo> characterInfo;
	generator->GetCharacterInfo(gAnsiCharset, characterInfo);

	std::vector<CharacterKerning> kerning;
	generator->GetKerning(kerning);
	
	int imageWidth = 1;
	int imageHeight = 1;
	int pixelArea = (int)ceil(WidthSum(characterInfo) * fontInfo.height);

	while (pixelArea != 0 || !LayoutText(NULL, characterInfo, fontInfo.height, imageWidth, imageHeight))
	{
		if (imageHeight == imageWidth)
		{
			imageHeight <<= 1;
		}
		else
		{
			imageWidth <<= 1;
		}

		if (pixelArea != 0)
		{
			pixelArea >>= 1;
		}
	}

	generator->SetImageSize(imageWidth, imageHeight);

	LayoutText(generator.get(), characterInfo, fontInfo.height, imageWidth, imageHeight);

	Texture2D::Ptr texture = resourceManager.GetGraphics()->CreateTexture2D(Vector2i(imageWidth, imageHeight), DataFormat(DataFormat::Int8UNorm, 1));

	vector<char> greyscaleBuffer(imageWidth * imageHeight);
	generator->CopyImageData(&greyscaleBuffer.front());
	texture->LoadMemory(&greyscaleBuffer.front());

	return Font::Ptr(new Font(characterInfo, kerning, texture));
}

float FontLoader::WidthSum(const std::vector<CharacterInfo>& character)
{
	float result = 0.0f;

	for (auto it = character.begin(); it != character.end(); ++it)
	{
		result += it->width;
	}

	return result;
}

bool FontLoader::LayoutText(FontGenerator* output, std::vector<CharacterInfo>& character, int fontHeight, int width, int height)
{
	int currentX = 0;
	int currentY = 0;

	for (auto it = character.begin(); it != character.end(); ++it)
	{
		if (currentX + it->width > width)
		{
			currentX = 0;
			currentY += fontHeight;

			if (currentY + fontHeight > height)
			{
				// the font doesn't fit in the image
				return false;
			}
		}
		
		if (output != NULL)
		{
			output->DrawUTF8Character(*it, Vector2i(currentX, currentY), fontHeight);
			it->textureOrigin = Vector2f((float)currentX / width, (float)currentY / height);
			it->textureSize = Vector2f(it->width / width, (float)fontHeight / height);
		}

		currentX += (int)ceil(it->width);
	}

	return true;
}

}