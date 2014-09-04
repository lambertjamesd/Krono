#pragma once

#include "DirectiveNode.h"
#include <string>
#include <stack>

namespace preproc
{

class IncludeHandler
{
public:
	IncludeHandler(const std::string& systemPath, const std::string& filePath);
	~IncludeHandler(void);
	
	void Open(IncludeNode::IncludeType includeType, const char* fileName, void** dataOut, size_t *dataSize);
	void Close(void* data);
private:
	std::stack<std::string> mFileStack;
	std::string mSystemPath;
};

}