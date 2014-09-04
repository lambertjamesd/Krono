#include "GLSLBundler.h"
#include "FileNotFoundException.h"
#include <iostream>

GLSLBundler::GLSLBundler(void)
{
}


GLSLBundler::~GLSLBundler(void)
{
}

std::vector<char> GLSLBundler::Process(const BundleDefinition& bundleDef) const
{
	std::ifstream input(bundleDef.GetFilename(ShaderLanguage::GLSL_4_4), std::ios::binary);

	if (input.is_open())
	{
		std::vector<char> result;
		input.seekg(0, std::ios::end);
		result.resize((unsigned int)input.tellg());
		input.seekg(0, std::ios::beg);

		if (result.size())
		{
			input.read(&result[0], result.size());
		}

		return result;
	}
	else
	{
		throw FileNotFoundException(std::string("Could not open file " + bundleDef.GetFilename(ShaderLanguage::GLSL_4_4)));
	}
}
