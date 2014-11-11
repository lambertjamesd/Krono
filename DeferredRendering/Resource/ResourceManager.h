#pragma once

#include <string>
#include <unordered_map>
#include "Core/Memory.h"
#include "ResourceLoader.h"
#include <fstream>
#include "LoadException.h"
#include "FileHelper.h"
#include "Mesh/Mesh.h"

namespace krono
{

class ResourceManager
{
public:
	typedef Auto<ResourceManager> Ptr;

	ResourceManager(Graphics* graphics);
	~ResourceManager(void);

	template <typename T>
	void AddLoader(Auto<ResourceLoader> loader)
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

		if (!FileHelper::DoesFileExist(path))
		{
			path = FileHelper::JoinPaths(mPathStack.back(), path);
		}

		std::ifstream fileInput(path, std::ios::in | std::ios::binary);

		if (!fileInput.is_open())
		{
			throw LoadException(filename, "Could not open file");
		}

		try
		{
			mPathStack.push_back(FileHelper::RemoveLastPathElement(path));
			Auto<T> result = LoadResource<T>(fileInput, internalName);
			if (internalName.length() > 0)
			{
				result->ResolveSource(path + "#" + internalName);
			}
			else
			{
				result->ResolveSource(path);
			}
			mPathStack.pop_back();
			return result;
		}
		catch (Exception& e)
		{
			mPathStack.pop_back();
			throw LoadException(filename, e.what());
		}
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

	Mesh::Ptr GetPlane();
	Mesh::Ptr GetSphere();

	Graphics* GetGraphics();
private:
	Graphics* mGraphics;
	std::unordered_map<size_t, Auto<ResourceLoader> > mLoaders;

	std::vector<std::string> mPathStack;

	void AddDefaultLoaders();
};

}
