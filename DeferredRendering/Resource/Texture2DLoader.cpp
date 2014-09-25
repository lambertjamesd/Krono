
#include "Texture2DLoader.h"
#include "FormatException.h"
#include "ResourceManager.h"
#include <FreeImage.h>

using namespace std;

namespace krono
{

Texture2DLoader::Texture2DLoader(void)
{
}


Texture2DLoader::~Texture2DLoader(void)
{
}


Auto<Object> Texture2DLoader::LoadResource(ResourceManager& resourceManager, istream& inputStream, const std::string& internalName)
{
	FreeImageIO io;
	fi_handle handle = (fi_handle)(&inputStream);

	io.read_proc = [] (void *buffer, unsigned size, unsigned count, fi_handle handle) {
		istream* input = (istream*)handle;
		unsigned bytes = size * count;

		unsigned totalRead = 0;
		unsigned loopRead = 0;

		do
		{
			input->read((char*)buffer + totalRead, bytes - totalRead);
			loopRead = (unsigned)input->gcount();
			totalRead += loopRead;
		} while (loopRead > 0 && totalRead < bytes);

		unsigned result = totalRead / size;
		unsigned overRead = totalRead % size;

		if (overRead > 0)
		{
			input->seekg(-(int)overRead, ios_base::cur);
		}

		return result;
	};

	io.write_proc = NULL;
	io.seek_proc = [] (fi_handle handle, long offset, int origin) {
		istream* input = (istream*)handle;

		ios_base::seekdir seekDir = 
			(origin == SEEK_END) ? ios_base::end : (
			(origin == SEEK_CUR) ? ios_base::cur : ios_base::beg);

		try
		{
			input->seekg(offset, seekDir);
			return (int)(input->fail() ? 1 : 0);
		}
		catch (ios_base::failure&)
		{
			return (int)1;
		}
	};

	io.tell_proc = [](fi_handle handle) {
		istream* input = (istream*)handle;
		return (long)input->tellg();
	};

	FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileTypeFromHandle(&io, handle, 0);
	
	if (imageFormat == FIF_UNKNOWN)
	{
		throw FormatException("Unrecogized image format");
	}

	FIBITMAP* bitmap = FreeImage_LoadFromHandle(imageFormat, &io, handle, 0);

	FREE_IMAGE_TYPE dataType = FreeImage_GetImageType(bitmap);

	DataFormat::Type formatType;
	size_t channelCount = 0;

	vector<unsigned char> textureData;

	Vector2i size(FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap));

	if (dataType == FIT_BITMAP)
	{
		formatType = DataFormat::Int8UNorm;
		channelCount = 4;

		FIBITMAP* bitmap32Bit = FreeImage_ConvertTo32Bits(bitmap);

		FreeImage_Unload(bitmap);

		bitmap = bitmap32Bit;

		textureData.resize(FreeImage_GetPitch(bitmap) * size.y);

		SwapChannels<unsigned char>(FreeImage_GetBits(bitmap), &textureData.front(), 0, 2, size.x * size.y, 4);
	}
	else
	{
		throw FormatException("Unsupported depth format");
	}

	Auto<Texture2D> result = resourceManager.GetGraphics()->CreateTexture2D(size, DataFormat(formatType, channelCount));

	result->LoadMemory(&textureData.front());
	result->GenerateMipmaps();

	if (bitmap != NULL)
	{
		FreeImage_Unload(bitmap);
		bitmap = NULL;
	}

	return result;
}

}
