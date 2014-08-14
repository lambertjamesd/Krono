
#include "DataFormat.h"


size_t DataFormat::gTypeSize[] = {sizeof(float), sizeof(unsigned char), sizeof(short), 3, sizeof(float), sizeof(long), 5};

DataFormat::DataFormat(Type type, size_t count) :
	type(type),
	count(count)
{
	static_assert((sizeof(gTypeSize) / sizeof(*gTypeSize)) == DataFormat::TypeCount, "Missing elements in gTypeSize");
}


DataFormat::~DataFormat(void)
{
}


size_t DataFormat::GetSize() const
{
	return gTypeSize[type] * count;
}
