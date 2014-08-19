#include "BundleDefinition.h"

#include "JSON/json.h"

BundleDefinition::BundleDefinition(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	ParseDefinition(file);
}


BundleDefinition::~BundleDefinition(void)
{

}

bool BundleDefinition::HasLanguage(BundleDefinition::ShaderLanguage language) const
{
	return mSources.find(language) != mSources.end();
}

std::vector<BundleDefinition::ShaderLanguage> BundleDefinition::GetBoundLanguages() const
{
	std::vector<BundleDefinition::ShaderLanguage> result;

	for (auto it = mSources.begin(); it != mSources.end(); ++it)
	{
		result.push_back(it->first);
	}

	return result;
}

std::string BundleDefinition::GetFilename(ShaderLanguage langauge) const
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
		ShaderLanguage langauge = LanguageNameToType(it->first);

		if (langauge != InvalidLanguage)
		{
			mSources[langauge] = it->second.ToString();
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

const char* BundleDefinition::gShaderLanguageNames[BundleDefinition::ShaderLanguage::LanguageCount] = {
	"HLSL 5",
	"GLSL 4.4"
};

BundleDefinition::ShaderLanguage BundleDefinition::LanguageNameToType(const std::string& name)
{
	for (size_t i = 0; i < BundleDefinition::ShaderLanguage::LanguageCount; ++i)
	{
		if (name == gShaderLanguageNames[i])
		{
			return static_cast<BundleDefinition::ShaderLanguage>(i);
		}
	}

	return BundleDefinition::InvalidLanguage;
}
