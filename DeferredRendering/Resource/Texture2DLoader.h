#pragma once

#include "ResourceLoader.h"

namespace krono
{

class Texture2DLoader :
	public ResourceLoader
{
public:
	Texture2DLoader(void);
	~Texture2DLoader(void);
	
	virtual Auto<Resource> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);
private:

	template <typename T>
	static void SwapChannels(const T* source, T* destination, size_t channelA, size_t channelB, size_t sampleCount, size_t stride)
	{
		for (size_t i = 0; i < sampleCount; ++i)
		{
			const T* pixelSource = source;
			T* pixelDest = destination;

			for (size_t channel = 0; channel < stride; ++channel)
			{
				if (channel == channelA)
				{
					pixelDest[channel] = pixelSource[channelB];
				}
				else if (channel == channelB)
				{
					pixelDest[channel] = pixelSource[channelA];
				}
				else
				{
					pixelDest[channel] = pixelSource[channel];
				}
			}

			source += stride;
			destination += stride;
		}
	}
};

}
