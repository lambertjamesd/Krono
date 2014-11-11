#pragma once

#include "Memory.h"

namespace krono
{

class Object
{
public:
	typedef Auto<Object> Ptr;
	typedef std::weak_ptr<Object> Ref;

	Object(void);
	virtual ~Object(void);
private:
};

}
