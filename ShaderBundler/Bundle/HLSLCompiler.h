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

