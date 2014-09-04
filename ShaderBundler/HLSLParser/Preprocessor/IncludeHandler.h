#pragma once

#include "DirectiveNode.h"
#include <string>
#include <stack>
#include <set>

namespace preproc
{

class IncludeHandler
{
public:
	IncludeHandler(const std::string& systemPath, const std::string& filePath);
	~IncludeHandler(void);
	
	void Open(IncludeNode::IncludeType includeType, const char* fileName, void** dataOut, size_t *dataSize);
	void Close(void* data);

	const std::set<std::string>& GetIncludedFiles() const;
private:
	std::stack<std::string> mFileStack;
	std::string mSystemPath;

	std::set<std::string> mIncludedFiles;
};

}
