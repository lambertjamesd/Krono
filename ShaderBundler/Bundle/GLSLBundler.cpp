#include "GLSLBundler.h"


GLSLBundler::GLSLBundler(void)
{
}


GLSLBundler::~GLSLBundler(void)
{
}

std::vector<char> GLSLBundler::Process(const BundleDefinition& bundleDef) const
{
	std::ifstream input(bundleDef.GetFilename(BundleDefinition::GLSL_4_4), std::ios::binary);

	std::vector<char> result;
	input.seekg(0, std::ios::end);
	result.resize((unsigned int)input.tellg());
	input.seekg(0, std::ios::beg);
	input.read(&result[0], result.size());

	return result;
}