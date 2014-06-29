#pragma once

#include <vector>
#include <string>
#include "../Core/Hash.h"

class Attribute
{
public:
	enum Type
	{
		Float,
		UInt8,
	};

	Attribute(const std::string& name, Type type, size_t count);
	Attribute(const std::string& name, Type type, size_t count, size_t index);
	~Attribute();
	

	const std::string& GetName() const;

	Type GetType() const;
	size_t GetCount() const;
	size_t GetIndex() const;
	
	// size in bytes
	size_t GetSize() const;
private:
	static size_t TypeSize[];

	std::string mName;
	Type mType;
	size_t mCount;
	size_t mIndex;
};

class InputLayout
{
public:
	InputLayout(void);
	~InputLayout(void);

	size_t GetAttributeCount() const;
	void AddAttribute(const Attribute& value);
	const Attribute& GetAttribute(size_t index) const;

	size_t GetStride() const;

	UINT32 GetSignature() const;
private:
	std::vector<Attribute> mAttributes;
	size_t mStride;
	HashUInt32 mSignature;
};

