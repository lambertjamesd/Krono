
#include "Hash.h"

HashUInt32::HashUInt32(UInt32 initialDigest) : Hash(&initialDigest)
{

}

HashUInt32::HashUInt32()
{

}

HashUInt32::~HashUInt32()
{

}
	
void HashUInt32::Update(const void* data, size_t length)
{
	Hash::Update(data, length);
}

UInt32 HashUInt32::GetDigest() const
{
	UInt32 result;
	Hash::GetDigest(&result);
	return result;
}