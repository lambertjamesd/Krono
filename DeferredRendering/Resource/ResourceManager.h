#pragma once

#include <string>
#include <unordered_map>
#include "Core/Memory.h"
#include "ResourceLoader.h"
#include <fstream>
#include <sstream>
#include "LoadException.h"
#include "FileHelper.h"
#include "Mesh/Mesh.h"

namespace krono
{

class WeakReference {
public:
	virtual ~WeakReference() {};

	virtual bool HasRef() {
		return false;
	}
private:
};

template <typename T>
class TemplatedWeakReference : public WeakReference {
public:
	~TemplatedWeakReference() {}
	TemplatedWeakReference(const Auto<T>& ptr) : mWeakRef(ptr) {

	}

	bool HasRef() {
		return !mWeakRef.expired();
	}

	Auto<T> GetRef() {
		return mWeakRef.lock();
	}
private:
	std::weak_ptr<T> mWeakRef;
};

typedef std::tuple<size_t, std::string> CacheKey;

struct CacheKeyHash : public std::unary_function<CacheKey, std::size_t>
{
	std::size_t operator()(const CacheKey& k) const
	{
		return std::get<0>(k) ^ std::hash<std::string>{}(std::get<1>(k));
	}
};

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
		auto cacheKey = std::make_tuple(typeid(T).hash_code(), filename);

		auto cachedValue = mCache.find(cacheKey);

		if (cachedValue != mCache.end() && cachedValue->second.HasRef()) {
			Auto<T> result = ((TemplatedWeakReference<T>*)(&cachedValue->second))->GetRef();

			if (result) {
				return result;
			}
		}

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

		if (path.length() == 0 && internalName.length() > 0)
		{
			std::istringstream emptyString(std::string(""));
			Auto<T> result = LoadResource<T>(emptyString, internalName);
			result->ResolveSource(internalName);
			return result;
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

			mCache.insert_or_assign(cacheKey, TemplatedWeakReference<T>(result));

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
	std::unordered_map<CacheKey, WeakReference, CacheKeyHash> mCache;

	std::vector<std::string> mPathStack;

	void AddDefaultLoaders();

	Mesh::Ptr mPlaneCache;
	Mesh::Ptr mSphereCache;
};

}
