#pragma once

#include "BundleDefinition.h"

#include <D3Dcompiler.h>
#include <vector>
#include <stack>

class HLSLIncludeHandler : public ID3DInclude
{
public:
	HLSLIncludeHandler(const std::string& systemPath, const std::string& filePath);
	~HLSLIncludeHandler(void);
	
    STDMETHOD(Open)(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
    STDMETHOD(Close)(THIS_ LPCVOID pData);

	static size_t LastSlash(const std::string& path);
	static std::string RemoveLastPathElement(const std::string& filename);
	static std::string JoinPaths(const std::string& basePath, const std::string& path);
private:
	std::stack<std::string> mFileStack;
	std::string mSystemPath;
};

class HLSLCompiler
{
public:
	HLSLCompiler(void);
	~HLSLCompiler(void);

	std::vector<char> Process(const BundleDefinition& bundleDef) const;
private:

	static const char* gShaderTargets[ShaderType::Count];
};

