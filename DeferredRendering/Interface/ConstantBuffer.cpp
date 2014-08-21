
#include "ConstantBuffer.h"

ConstantBufferLayout::ConstantBufferLayout(void)
{

}

ConstantBufferLayout::~ConstantBufferLayout(void)
{

}

void ConstantBufferLayout::MarkSpecialType(ConstantBufferLayout::Type type, size_t offset)
{
	mSpecialTypes.push_back(std::make_pair(type, offset));
}

const std::vector<std::pair<ConstantBufferLayout::Type, size_t> >& ConstantBufferLayout::GetSpecialTypes() const
{
	return mSpecialTypes;
}

ConstantBuffer::ConstantBuffer(const ConstantBufferLayout& layout) :
	mLayout(layout)
{
}


ConstantBuffer::~ConstantBuffer(void)
{
}