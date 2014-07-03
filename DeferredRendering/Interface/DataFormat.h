#pragma once

struct DataFormat
{
	enum Type
	{
		Float,
		UInt8,
		TypeCount
	};

	DataFormat(Type type, size_t count);
	~DataFormat(void);

	Type type;
	size_t count;

	size_t GetSize() const;

private:
	static size_t gTypeSize[];
};

