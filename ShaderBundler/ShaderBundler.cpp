// ShaderBundler.cpp : Defines the entry point for the console application.
//

#include <string>
#include <iostream>

#include "Bundle\BundleDefinition.h"
#include "Bundle\HLSLCompiler.h"
#include "Bundle\GLSLBundler.h"
#include "Bundle\Bundle.h"

std::string GetOutputFilename(const std::string& inputFilename)
{
	size_t extensionPosition = inputFilename.find_last_of('.');
	return inputFilename.substr(0, extensionPosition) + ".shader";
}

void ProccessFile(const char* filename)
{
	BundleDefinition bundleDef(filename);
	Bundle bundle(bundleDef);

	std::cout << "Processing file " << filename << std::endl;

	try
	{
		if (bundleDef.HasLanguage(BundleDefinition::HLSL_5))
		{
			HLSLCompiler compiler;
			bundle.AddShader(BundleDefinition::HLSL_5, compiler.Process(bundleDef));
		}

		if (bundleDef.HasLanguage(BundleDefinition::GLSL_4_4))
		{
			GLSLBundler bundler;
			bundle.AddShader(BundleDefinition::GLSL_4_4, bundler.Process(bundleDef));
		}

		std::string outputFilename = GetOutputFilename(filename);

		std::cout << "Writing shader " << outputFilename << std::endl;

		std::ofstream outputFile(outputFilename, std::ios::binary);
		bundle.Write(outputFile);
		outputFile.close();
	}
	catch (std::exception& exception)
	{
		std::cerr << "Error processing shader: " << exception.what();
	}
}

int main(int argc, char* argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		ProccessFile(argv[i]);
	}

	std::cin.get();

	return 0;
}

