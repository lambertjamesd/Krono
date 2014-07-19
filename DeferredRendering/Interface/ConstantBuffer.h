#pragma once

#include <vector>

class ConstantBufferLayout
{
public:
	ConstantBufferLayout(void);
	~ConstantBufferLayout(void);

	void MarkProjectionMatrix(size_t offset);

	const std::vector<size_t> GetProjectionMatrixPositions() const;
private:
	std::vector<size_t> mProjectionMatrixPositions;
};

class ConstantBuffer
{
public:
	~ConstantBuffer(void);

	template <typename T>
	void Set(const T& value)
	{
		Set(&value, sizeof(T));
	}

	virtual void Set(const void* data, size_t size) = 0;
protected:
	ConstantBuffer(const ConstantBufferLayout& layout);
	ConstantBufferLayout mLayout;
};

