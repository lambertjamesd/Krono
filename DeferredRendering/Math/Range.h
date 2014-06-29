#pragma once


template <typename T>
class Range
{
public:
	Range(void) : start(), length() {};
	Range(const T& start, const T& length) : start(start), length(length) {}
	~Range(void) {};

	T start;
	T length;
private:
};

typedef Range<int> Rangei;
typedef Range<float> Rangef;