#pragma once

#include <stddef.h>
#include <string>

class FileHelper
{
public:
	static size_t LastSlash(const std::string& path);
	static std::string RemoveLastPathElement(const std::string& filename);
	static std::string JoinPaths(const std::string& basePath, const std::string& path);
};
