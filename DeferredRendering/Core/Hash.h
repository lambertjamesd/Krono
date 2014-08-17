#pragma once

#include "Types.h"
#include <string.h>

template <size_t tDigestSize>
class Hash
{
public:
	Hash(const void* initialDigest) :
		mCurrentIndex(0)
	{
		memcpy(mDigest, initialDigest, tDigestSize);
	}

	Hash(void) :
		mCurrentIndex(0)
	{
		memset(mDigest, 0, tDigestSize);
	}

	~Hash(void) {}
	
	template <typename T>
	void Update(const T& data)
	{
		Update(&data, sizeof(T));
	}

	void Update(const void* data, size_t length)
	{
		static const unsigned char lookupTable[256] = {
			// 256 values 0-255 in any (random) order suffices
			98,  6, 85,150, 36, 23,112,164,135,207,169,  5, 26, 64,165,219, //  1
			61, 20, 68, 89,130, 63, 52,102, 24,229,132,245, 80,216,195,115, //  2
			90,168,156,203,177,120,  2,190,188,  7,100,185,174,243,162, 10, //  3
			237, 18,253,225,  8,208,172,244,255,126,101, 79,145,235,228,121, //  4
			123,251, 67,250,161,  0,107, 97,241,111,181, 82,249, 33, 69, 55, //  5
			59,153, 29,  9,213,167, 84, 93, 30, 46, 94, 75,151,114, 73,222, //  6
			197, 96,210, 45, 16,227,248,202, 51,152,252,125, 81,206,215,186, //  7
			39,158,178,187,131,136,  1, 49, 50, 17,141, 91, 47,129, 60, 99, //  8
			154, 35, 86,171,105, 34, 38,200,147, 58, 77,118,173,246, 76,254, //  9
			133,232,196,144,198,124, 53,  4,108, 74,223,234,134,230,157,139, // 10
			189,205,199,128,176, 19,211,236,127,192,231, 70,233, 88,146, 44, // 11
			183,201, 22, 83, 13,214,116,109,159, 32, 95,226,140,220, 57, 12, // 12
			221, 31,209,182,143, 92,149,184,148, 62,113, 65, 37, 27,106,166, // 13
			3, 14,204, 72, 21, 41, 56, 66, 28,193, 40,217, 25, 54,179,117, // 14
			238, 87,240,155,180,170,242,212,191,163, 78,218,137,194,175,110, // 15
			43,119,224, 71,122,142, 42,160,104, 48,247,103, 15, 11,138,239  // 16
		};

		const unsigned char* dataAsBytes = (const unsigned char*)data;

		for (size_t i = 0; i < length; ++i)
		{
			mDigest[mCurrentIndex] = lookupTable[mDigest[mCurrentIndex] ^ dataAsBytes[i]];
			mCurrentIndex = (mCurrentIndex + 1) % tDigestSize;
		}
	}

	void GetDigest(void* output, size_t length = tDigestSize) const
	{
		if (length > tDigestSize)
		{
			length = tDigestSize;
		}

		memcpy(output, mDigest, length);
	}
private:
	unsigned char mDigest[tDigestSize];
	size_t mCurrentIndex;
};

class HashUInt32 : private Hash<4>
{
public:
	HashUInt32(UInt32 initialDigest);
	HashUInt32();
	~HashUInt32();
	
	template <typename T>
	void Update(const T& data)
	{
		Hash::Update<T>(data);
	}

	void Update(const void* data, size_t length);
	UInt32 GetDigest() const;
private:
};