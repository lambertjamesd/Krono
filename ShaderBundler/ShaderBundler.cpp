// ShaderBundler.cpp : Defines the entry point for the console application.
//

#include <string>
#include <iostream>
#include <cstring>

#include "Bundle/BundleDefinition.h"
#include "Bundle/GLSLBundler.h"
#include "Bundle/Bundle.h"

#ifdef USE_DX11
#include "Bundle/HLSLCompiler.h"
#endif

std::string GetOutputFilename(const std::string& inputFilename)
{
	size_t extensionPosition = inputFilename.find_last_of('.');
	return inputFilename.substr(0, extensionPosition) + ".shader";
}

void MakeDependencies(const char* filename)
{
	BundleDefinition bundleDef(filename);

	std::vector<ShaderLanguage::Type> languages = bundleDef.GetBoundLanguages();

	std::cout << filename << ": ";

	for (auto it = languages.begin(); it != languages.end(); ++it)
	{
		std::cout << bundleDef.GetFilename(*it) << " ";
	}

	std::cout << std::endl;
}

void ProccessFile(const char* filename)
{
	BundleDefinition bundleDef(filename);
	Bundle bundle(bundleDef);

	std::cout << "Processing file " << filename << std::endl;

	try
	{
#ifdef USE_DX11
		if (bundleDef.HasLanguage(ShaderLanguage::HLSL_5))
		{
			HLSLCompiler compiler;
			bundle.AddShader(ShaderLanguage::HLSL_5, compiler.Process(bundleDef));
		}
#endif

		if (bundleDef.HasLanguage(ShaderLanguage::GLSL_4_4))
		{
			GLSLBundler bundler;
			bundle.AddShader(ShaderLanguage::GLSL_4_4, bundler.Process(bundleDef));
		}

		std::string outputFilename = GetOutputFilename(filename);

		std::cout << "Writing shader " << outputFilename << std::endl;

		std::ofstream outputFile(outputFilename.c_str(), std::ios::binary);
		bundle.Write(outputFile);
		outputFile.close();
	}
	catch (std::exception& exception)
	{
		std::cerr << "Error processing shader: " << exception.what() << std::endl;
	}
}

int main(int argc, char* argv[])
{
	if (argc > 1 && strcmp(argv[1], "-MM") == 0)
	{
		for (int i = 2; i < argc; ++i)
		{
			MakeDependencies(argv[i]);
		}
	}
	else
	{
		for (int i = 1; i < argc; ++i)
		{
			ProccessFile(argv[i]);
		}
	}

	return 0;
}

