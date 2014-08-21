#pragma once

#include "Resource/Resource.h"
#include "JSON/json.h"

namespace krono
{

class JsonDocument : public Resource
{
public:
	JsonDocument(const std::string& source);
	~JsonDocument(void);

	json::Value& GetRoot();
	const json::Value& GetRoot() const;
private:

	json::Value mRoot;
};

}
