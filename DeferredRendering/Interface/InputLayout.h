#pragma once

#include <vector>
#include <string>
#include "Core/Hash.h"
#include "DataFormat.h"

class Attribute
{
public:
	Attribute(const std::string& name, const DataFormat& dataFormat);
	Attribute(const std::string& name, const DataFormat& dataFormat, size_t index);
	~Attribute();
	

	const std::string& GetName() const;

	const DataFormat& GetFormat() const;
	size_t GetIndex() const;
private:
	static size_t TypeSize[];

	std::string mName;
	DataFormat mFormat;
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

	UInt32 GetSignature() const;
private:
	std::vector<Attribute> mAttributes;
	size_t mStride;
	HashUInt32 mSignature;
};

