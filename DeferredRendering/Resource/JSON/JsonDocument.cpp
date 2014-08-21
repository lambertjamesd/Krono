#include "JsonDocument.h"

namespace krono
{

JsonDocument::JsonDocument(const std::string& source) :
	mRoot(json::Deserialize(source))
{
}


JsonDocument::~JsonDocument(void)
{
}

json::Value& JsonDocument::GetRoot()
{
	return mRoot;
}

const json::Value& JsonDocument::GetRoot() const
{
	return mRoot;
}

}
