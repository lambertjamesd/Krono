#include "Bundle.h"


Bundle::Bundle(const BundleDefinition& bundleDef) :
	mType(bundleDef.GetType())
{
}


Bundle::~Bundle(void)
{
}

void Bundle::AddShader(BundleDefinition::ShaderLanguage language, const std::vector<char>& data)
{
	mShaders[language] = data;
}

const std::string Bundle::gFileHeader = "SHDR";

void Bundle::Write(std::ostream& output) const
{
	output.write(gFileHeader.c_str(), gFileHeader.length());
	WriteInt(output, static_cast<short>(mType));
	WriteInt(output, static_cast<short>(mShaders.size()));

	size_t headerSize = (size_t)output.tellp();
	size_t offsetDataSize = (sizeof(short) + 2 * sizeof(long)) * mShaders.size();

	for (auto it = mShaders.begin(); it != mShaders.end(); ++it)
	{
		WriteInt(output, static_cast<short>(it->first));
		WriteInt(output, static_cast<long>(headerSize + offsetDataSize + GetShaderOffset(it->first)));
		WriteInt(output, static_cast<long>(it->second.size()));
	}
	
	for (auto it = mShaders.begin(); it != mShaders.end(); ++it)
	{
		output.write(&it->second.front(), it->second.size());
	}
}


size_t Bundle::GetShaderOffset(BundleDefinition::ShaderLanguage language) const
{
	size_t result = 0;

	for (auto it = mShaders.begin(); it != mShaders.end(); ++it)
	{
		if (language == it->first)
		{
			return result;
		}
		else
		{
			result += it->second.size();
		}
	}

	return -1;
}