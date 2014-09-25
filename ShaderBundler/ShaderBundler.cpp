// ShaderBundler.cpp : Defines the entry point for the console application.
//

#include <string>
#include <iostream>
#include <cstring>
#include <sstream>

#include "Bundle/BundleDefinition.h"
#include "Bundle/GLSLBundler.h"
#include "Bundle/Bundle.h"

#include "HLSLParser/HLSLTokenizer.h"
#include "HLSLParser/HLSLParser.h"
#include "HLSLParser/Preprocessor/Preprocessor.h"
#include "HLSLParser/GLSLGenerator/GLSLGenerator.h"
#include "HLSLParser/HLSLParserException.h"
#include "HLSLParser/ShaderInputBuilder.h"

#ifdef USE_DX11
#include "Bundle/HLSLCompiler.h"
#endif

using namespace std;

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
		std::string fileName = bundleDef.GetFilename(*it);
		std::cout << fileName << " ";

		if (*it == ShaderLanguage::HLSL_5)
		{
			try
			{
				preproc::PreprocessResult includes = preproc::Preprocessor::PreprocessFile(fileName, map<string,string>());

				for (auto includeIt = includes.includedFiles.begin(); includeIt != includes.includedFiles.end(); ++includeIt)
				{
					std::cout << *includeIt << " ";
				}
			}
			catch (preproc::Exception&)
			{

			}
		}
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
		if (bundleDef.HasLanguage(ShaderLanguage::HLSL_5))
		{
			map<string,string> macros;
			macros["OPENGL"] = "1";
			unique_ptr<HLSLNode> file = move(HLSLParser::ProcessFile(bundleDef.GetFilename(ShaderLanguage::HLSL_5), bundleDef.GetType(), bundleDef.GetEntryPoint(), macros));

			if (bundleDef.CrossCompileHLSL())
			{
				try
				{
					ostringstream result;
					GLSLGenerator::ProcessFile(*file, bundleDef.GetType(), bundleDef.GetEntryPoint(), result);
					string stringResult(result.str());
					vector<char> vectorResult(stringResult.begin(), stringResult.end());
					bundle.AddShader(ShaderLanguage::GLSL_4_4, vectorResult);
				}
				catch (HLSLParserException& exception)
				{
					cerr << exception.what() << endl;
				}
			}

			ostringstream inputLayout;
			ShaderInputBuilder::Process(*file, inputLayout);
			string layoutAsString = inputLayout.str();
			bundle.AddShader(ShaderLanguage::ShaderInputLayout, vector<char>(layoutAsString.begin(), layoutAsString.end()));
		}
		else if (bundleDef.HasLanguage(ShaderLanguage::GLSL_4_4))
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
	HLSLTokenizer::Test();
	preproc::Preprocessor::Test();
	HLSLParser::Test();

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

	//ProccessFile("C:\\Users\\James\\Documents\\Visual Studio 2012\\Projects\\DeferredRendering\\TowerDefense\\Media\\Engine\\Compositor\\SSAO\\SSAO.json");

	//ProccessFile("/home/lambertjamesd/Documents/Krono/TowerDefense/Media/Shaders/Bundle/PointLight.json");

	return 0;
}