
#include "DataFormat.h"


namespace krono
{

size_t DataFormat::gTypeSize[DataFormat::TypeCount] = {sizeof(float), sizeof(unsigned char), sizeof(short), 3, sizeof(float), sizeof(long), 5};

DataFormat::DataFormat() :
	type(Type::TypeCount),
	count(0)
{

}

DataFormat::DataFormat(Type type, size_t count) :
	type(type),
	count(count)
{

}


DataFormat::~DataFormat(void)
{
}


size_t DataFormat::GetSize() const
{
	return gTypeSize[type] * count;
}

bool DataFormat::IsDepthFormat() const
{
	return IsDepthFormat(type);
}

bool DataFormat::IsDepthFormat(Type type)
{
	return type >= Depth16 && type <= Depth32FS8;
}

}
