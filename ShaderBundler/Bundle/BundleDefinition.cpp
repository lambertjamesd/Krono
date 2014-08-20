#include "BundleDefinition.h"
#include "FileHelper.h"

#include "JSON/json.h"

BundleDefinition::BundleDefinition(const std::string& filename) :
	mBaseFilename(FileHelper::RemoveLastPathElement(filename))
{
	std::ifstream file(filename, std::ios::binary);
	ParseDefinition(file);
}


BundleDefinition::~BundleDefinition(void)
{

}

bool BundleDefinition::HasLanguage(ShaderLanguage::Type language) const
{
	return mSources.find(language) != mSources.end();
}

std::vector<ShaderLanguage::Type> BundleDefinition::GetBoundLanguages() const
{
	std::vector<ShaderLanguage::Type> result;

	for (auto it = mSources.begin(); it != mSources.end(); ++it)
	{
		result.push_back(it->first);
	}

	return result;
}

std::string BundleDefinition::GetFilename(ShaderLanguage::Type langauge) const
{
	auto result = mSources.find(langauge);

	if (result == mSources.end())
	{
		return "";
	}
	else
	{
		return result->second;
	}
}

ShaderType::Type BundleDefinition::GetType() const
{
	return mType;
}

const std::string& BundleDefinition::GetEntryPoint() const
{
	return mEntryPoint;
}

void BundleDefinition::ParseDefinition(std::istream& input)
{
	std::string contents;
	input.seekg(0, std::ios::end);
	contents.resize((unsigned int)input.tellg());
	input.seekg(0, std::ios::beg);
	input.read(&contents[0], contents.size());

	json::Object root = json::Deserialize(contents).ToObject();

	json::Object source = root["sources"];

	for (auto it = source.begin(); it != source.end(); ++it)
	{
		ShaderLanguage::Type langauge = LanguageNameToType(it->first);

		if (langauge != ShaderLanguage::Invalid)
		{
			mSources[langauge] = FileHelper::JoinPaths(mBaseFilename, it->second.ToString());
		}
	}

	mType = ShaderTypeNameToType(root["type"].ToString("invalid"));
	mEntryPoint = root["entryPoint"].ToString("main");
}

const char* BundleDefinition::gShaderTypes[ShaderType::Count] = {
	"vertex",
	"hull",
	"domain",
	"geometry",
	"pixel",
	"compute"
};

ShaderType::Type BundleDefinition::ShaderTypeNameToType(const std::string& name)
{
	for (size_t i = 0; i < ShaderType::Count; ++i)
	{
		if (name == gShaderTypes[i])
		{
			return static_cast<ShaderType::Type>(i);
		}
	}

	return ShaderType::Invalid;
}

const char* BundleDefinition::gShaderLanguageNames[ShaderLanguage::Count] = {
	"HLSL 5",
	"GLSL 4.4"
};

ShaderLanguage::Type BundleDefinition::LanguageNameToType(const std::string& name)
{
	for (size_t i = 0; i < ShaderLanguage::Count; ++i)
	{
		if (name == gShaderLanguageNames[i])
		{
			return static_cast<ShaderLanguage::Type>(i);
		}
	}

	return ShaderLanguage::Invalid;
}
