#include "HLSLCompiler.h"
#include "BundlerFileHelper.h"
#include "FileNotFoundException.h"

#include <iostream>

HLSLIncludeHandler::HLSLIncludeHandler(const std::string& systemPath, const std::string& filePath) :
	mSystemPath(systemPath)
{
	mFileStack.push(BundlerFileHelper::RemoveLastPathElement(filePath));
}

HLSLIncludeHandler::~HLSLIncludeHandler(void)
{

}

HRESULT HLSLIncludeHandler::Close(LPCVOID pData)
{
	delete [] (char*)pData;

	mFileStack.pop();

	return S_OK;
}

HRESULT HLSLIncludeHandler::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{
	std::string absolutePath;

	switch (IncludeType)
	{
	case D3D_INCLUDE_LOCAL:
		{
			absolutePath = BundlerFileHelper::JoinPaths(mFileStack.top(), pFileName);
		}
		break;
	case D3D_INCLUDE_SYSTEM:
		{
			absolutePath = BundlerFileHelper::JoinPaths(mSystemPath, pFileName);
		}
	}

	std::ifstream input(absolutePath, std::ios::binary);

	if (!input.is_open())
	{
		return STG_E_FILENOTFOUND;
	}

	input.seekg(0, std::ios::end);
	std::streamoff fileSize = input.tellg();
	char* contents = new char[(unsigned)fileSize];
	input.seekg(0, std::ios::beg);
	input.read(contents, fileSize);

	*ppData = (LPCVOID)contents;
	*pBytes = (UINT)fileSize;

	mFileStack.push(BundlerFileHelper::RemoveLastPathElement(absolutePath));

	return S_OK;

	return STG_E_FILENOTFOUND;
}

HLSLCompiler::HLSLCompiler(void)
{
}


HLSLCompiler::~HLSLCompiler(void)
{
}

const char* HLSLCompiler::gShaderTargets[ShaderType::Count] = {
	"vs_5_0",
	"hs_5_0",
	"ds_5_0",
	"gs_5_0",
	"ps_5_0",
	"cs_5_0",
};

std::vector<char> HLSLCompiler::Process(const BundleDefinition& bundleDef) const
{
	std::string filename = bundleDef.GetFilename(ShaderLanguage::HLSL_5);

	std::ifstream input(filename, std::ios::binary);

	if (!input.is_open())
	{
		throw FileNotFoundException(std::string("Could not open file " + bundleDef.GetFilename(ShaderLanguage::HLSL_5)));
	}

	std::string contents;
	input.seekg(0, std::ios::end);
	contents.resize((unsigned int)input.tellg());
	input.seekg(0, std::ios::beg);
	input.read(&contents[0], contents.size());

	ID3DBlob* data;
	ID3DBlob* error;

	HLSLIncludeHandler includeHandler("", filename);

	HRESULT compileResult = D3DCompile(
		&contents.front(),
		contents.size(),
		bundleDef.GetFilename(ShaderLanguage::HLSL_5).c_str(),
		NULL,
		&includeHandler,
		bundleDef.GetEntryPoint().c_str(),
		gShaderTargets[bundleDef.GetType()],
		0,
		0,
		&data,
		&error);

	if (FAILED(compileResult))
	{
		std::string errorMessage((char*)error->GetBufferPointer(), error->GetBufferSize());

		error->Release();

		std::cerr << errorMessage << std::endl;

		throw std::exception("Could not compile HLSL shader");
	}

	std::vector<char> result;

	result.resize(data->GetBufferSize());
	result.assign((char*)data->GetBufferPointer(), (char*)data->GetBufferPointer() + data->GetBufferSize());

	data->Release();

	return result;
}
