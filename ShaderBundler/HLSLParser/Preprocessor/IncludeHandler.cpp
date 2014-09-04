#include "IncludeHandler.h"
#include "Exception.h"
#include <fstream>

#include "../../Bundle/BundlerFileHelper.h"

namespace preproc
{

IncludeHandler::IncludeHandler(const std::string& systemPath, const std::string& filePath) :
	mSystemPath(systemPath)
{
	mFileStack.push(BundlerFileHelper::RemoveLastPathElement(filePath));
}


IncludeHandler::~IncludeHandler(void)
{
}

void IncludeHandler::Close(void* data)
{
	delete [] (char*)data;
	mFileStack.pop();
}

void IncludeHandler::Open(IncludeNode::IncludeType includeType, const char* fileName, void** dataOut, size_t *dataSize)
{
	std::string absolutePath;

	switch (includeType)
	{
	case IncludeNode::LocalInclude:
		{
			absolutePath = BundlerFileHelper::JoinPaths(mFileStack.top(), fileName);
		}
		break;
	case IncludeNode::SystemInclude:
		{
			absolutePath = BundlerFileHelper::JoinPaths(mSystemPath, fileName);
		}
	}

	std::ifstream input(absolutePath, std::ios::binary);

	if (!input.is_open())
	{
		std::string message;
		message += "Could not open file ";
		message += absolutePath;
		throw Exception(message.c_str());
	}

	input.seekg(0, std::ios::end);
	std::streamoff fileSize = input.tellg();
	char* contents = new char[(unsigned)fileSize];
	input.seekg(0, std::ios::beg);
	input.read(contents, fileSize);

	*dataOut = (void*)contents;
	*dataSize = (size_t)fileSize;

	mFileStack.push(BundlerFileHelper::RemoveLastPathElement(absolutePath));
}

}