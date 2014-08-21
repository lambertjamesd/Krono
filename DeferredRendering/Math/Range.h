#pragma once


namespace krono
{

template <typename T>
class Range
{
public:
	Range(void) : start(), end() {};
	Range(const T& start, const T& end) : start(start), end(end) {}
	~Range(void) {};

	T start;
	T end;
private:
};

typedef Range<int> Rangei;
typedef Range<float> Rangef;

}
