#pragma once

struct DataFormat
{
	enum Type
	{
		Float,
		Int8UNorm,

		Depth16,
		Depth24,
		Depth32F,
		Depth24S8,
		Depth32FS8,

		TypeCount
	};

	DataFormat(Type type, size_t count);
	~DataFormat(void);

	Type type;
	size_t count;

	size_t GetSize() const;

private:
	static size_t gTypeSize[DataFormat::TypeCount];
};

