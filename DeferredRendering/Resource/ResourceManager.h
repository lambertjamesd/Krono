#pragma once

#include <string>
#include <unordered_map>
#include "../Core/Memory.h"
#include "ResourceLoader.h"
#include <fstream>

class ResourceManager
{
public:
	ResourceManager(Graphics* graphics);
	~ResourceManager(void);

	template <typename T>
	void AddLoader(Auto<ResourceLoader>& loader)
	{
		mLoaders[typeid(T).hash_code()] = loader;
	}
	
	
	template <typename T>
	Auto<T> LoadResource(const std::string& filename)
	{
		std::string path;
		std::string internalName;

		size_t hashPosition = filename.find('#');

		if (hashPosition == std::string::npos)
		{
			path = filename;
		}
		else
		{
			path = filename.substr(0, hashPosition);
			internalName = filename.substr(hashPosition + 1);
		}

		std::ifstream fileInput(path);
		return LoadResource<T>(fileInput, internalName);
	}
	
	template <typename T>
	Auto<T> LoadResource(std::istream& inputStream, const std::string& internalName)
	{
		auto loader = mLoaders.find(typeid(T).hash_code());

		if (loader != mLoaders.end())
		{
			return std::dynamic_pointer_cast<T>(loader->second->LoadResource(*this, inputStream, internalName));
		}
		else
		{
			return Auto<T>(NULL);
		}
	}

	Graphics* GetGraphics();
private:
	Graphics* mGraphics;
	std::unordered_map<size_t, Auto<ResourceLoader> > mLoaders;

	void AddDefaultLoaders();
};

