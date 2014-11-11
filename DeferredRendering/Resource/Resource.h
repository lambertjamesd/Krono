#pragma once

#include "Core/Object.h"
#include "Core/Memory.h"
#include <string>

namespace krono
{

class Resource : public Object
{
public:
	typedef Auto<Resource> Ptr;

	Resource(void);
	virtual ~Resource(void);

	const std::string& GetSource() const;
	void ResolveSource(const std::string& source);
private:
	std::string mSource;
};

}
