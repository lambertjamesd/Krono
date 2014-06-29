#include "stdafx.h"
#include "InputLayout.h"


Attribute::Attribute(const std::string& name, Type type, size_t count) :
		mName(name),
		mType(type),
		mCount(count),
		mIndex(0)
{

}


Attribute::Attribute(const std::string& name, Type type, size_t count, size_t index) :
		mName(name),
		mType(type),
		mCount(count),
		mIndex(index)
{

}

Attribute::~Attribute()
{

}

const std::string& Attribute::GetName() const
{
	return mName;
}

Attribute::Type Attribute::GetType() const
{
	return mType;
}

size_t Attribute::GetCount() const
{
	return mCount;
}

size_t Attribute::GetIndex() const
{
	return mIndex;
}

size_t Attribute::GetSize() const
{
	return mCount * TypeSize[mType];
}

size_t Attribute::TypeSize[] = {sizeof(float), sizeof(unsigned char)};


InputLayout::InputLayout(void) :
	mStride(0)
{
}


InputLayout::~InputLayout(void)
{
}


void InputLayout::AddAttribute(const Attribute& value)
{
	mAttributes.push_back(value);
	mStride += value.GetSize();

	mSignature.Update(value.GetName().c_str(), value.GetName().length());

	size_t sizeTmp = value.GetCount();
	mSignature.Update(&sizeTmp, sizeof(size_t));
	sizeTmp = value.GetSize();
	mSignature.Update(&sizeTmp, sizeof(size_t));
	Attribute::Type type = value.GetType();
	mSignature.Update(&type, sizeof(type));
}

size_t InputLayout::GetAttributeCount() const
{
	return mAttributes.size();
}

const Attribute& InputLayout::GetAttribute(size_t index) const
{
	return mAttributes[index];
}

size_t InputLayout::GetStride() const
{
	return mStride;
}

UINT32 InputLayout::GetSignature() const
{
	return mSignature.GetDigest();
}