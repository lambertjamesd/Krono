
#include "BundlerFileHelper.h"
#include <cstring>

size_t BundlerFileHelper::LastSlash(const std::string& path)
{
	size_t result = path.find_last_of('/');

	if (result != std::string::npos)
	{
		return result;
	}
	else
	{
		return path.find_last_of('\\');
	}
}

std::string BundlerFileHelper::RemoveLastPathElement(const std::string& filename)
{
	size_t slashPos = LastSlash(filename);

	if (slashPos != std::string::npos)
	{
		return filename.substr(0, slashPos);
	}
	else
	{
		return "";
	}
}

std::string BundlerFileHelper::JoinPaths(const std::string& basePath, const std::string& path)
{
	std::string modifiedBase = basePath;
	std::string modifedPath = path;

	while (modifiedBase.length() > 0 && modifedPath.length() > 0 && modifedPath[0] == '.')
	{
		if (modifedPath.length() > 1 && 
			(modifedPath[1] == '/' || modifedPath[1] == '\\'))
		{
			modifedPath = modifedPath.substr(strlen("./"));
		}
		else if (modifedPath.length() > 2 && modifedPath[1] == '.' &&
			(modifedPath[2] == '/' || modifedPath[2] == '\\'))
		{
			modifedPath = modifedPath.substr(strlen("../"));
			modifiedBase = RemoveLastPathElement(modifiedBase);
		}
	}

	if (modifiedBase.length() > 0)
	{
		return modifiedBase + '/' + modifedPath;
	}
	else
	{
		return modifedPath;
	}
}
