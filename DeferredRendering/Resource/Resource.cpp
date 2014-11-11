
#include "Resource.h"
#include <cassert>

namespace krono
{

Resource::Resource(void)
{
}


Resource::~Resource(void)
{
}

const std::string& Resource::GetSource() const
{
	return mSource;
}

void Resource::ResolveSource(const std::string& source)
{
	assert(mSource.length() == 0);
	mSource = source;
}

}
