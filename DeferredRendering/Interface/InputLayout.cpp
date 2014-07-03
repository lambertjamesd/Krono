#include "stdafx.h"
#include "InputLayout.h"


Attribute::Attribute(const std::string& name, const DataFormat& format) :
		mName(name),
		mFormat(format),
		mIndex(0)
{

}


Attribute::Attribute(const std::string& name, const DataFormat& format, size_t index) :
		mName(name),
		mFormat(format),
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


const DataFormat& Attribute::GetFormat() const
{
	return mFormat;
}

size_t Attribute::GetIndex() const
{
	return mIndex;
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
	mStride += value.GetFormat().GetSize();

	mSignature.Update(value.GetName().c_str(), value.GetName().length());

	size_t sizeTmp = value.GetFormat().count;
	mSignature.Update(&sizeTmp, sizeof(size_t));
	sizeTmp = value.GetFormat().GetSize();
	mSignature.Update(&sizeTmp, sizeof(size_t));
	DataFormat::Type type = value.GetFormat().type;
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