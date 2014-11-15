#pragma once

#include <stddef.h>
#include <string>

namespace krono
{

class FileHelper
{
public:
	static size_t LastSlash(const std::string& path);
	static std::string RemoveLastPathElement(const std::string& filename);
	static std::string LastPathElement(const std::string& filename);

	static std::string JoinPaths(const std::string& basePath, const std::string& path);

	static std::string Extension(const std::string& filename);
	static std::string RemoveExtension(const std::string& filename);

	static bool DoesFileExist(const std::string& filename);
};

}