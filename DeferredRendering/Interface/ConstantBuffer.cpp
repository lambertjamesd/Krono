#include "stdafx.h"
#include "ConstantBuffer.h"

ConstantBufferLayout::ConstantBufferLayout(void)
{

}

ConstantBufferLayout::~ConstantBufferLayout(void)
{

}

void ConstantBufferLayout::MarkProjectionMatrix(size_t offset)
{
	mProjectionMatrixPositions.push_back(offset);
}

const std::vector<size_t> ConstantBufferLayout::GetProjectionMatrixPositions() const
{
	return mProjectionMatrixPositions;
}

ConstantBuffer::ConstantBuffer(const ConstantBufferLayout& layout) :
	mLayout(layout)
{
}


ConstantBuffer::~ConstantBuffer(void)
{
}