#include "stdafx.h"
#include "Hash.h"

HashUInt32::HashUInt32(UINT32 initialDigest) : Hash(&initialDigest)
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

UINT32 HashUInt32::GetDigest() const
{
	UINT32 result;
	Hash::GetDigest(&result);
	return result;
}