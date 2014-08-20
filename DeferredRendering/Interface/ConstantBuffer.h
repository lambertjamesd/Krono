#pragma once

#include <vector>
#include <utility>
#include "Core/Types.h"

class ConstantBufferLayout
{
public:
	enum Type
	{
		TypeProjectionMatrix,
		TypeInvProjectionMatrix,
	};

	ConstantBufferLayout(void);
	~ConstantBufferLayout(void);

	void MarkSpecialType(Type type, size_t offset);

	const std::vector<std::pair<Type, size_t> >& GetSpecialTypes() const;
private:
	std::vector<std::pair<Type, size_t> > mSpecialTypes;
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

