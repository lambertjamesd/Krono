#pragma once

namespace TokenType
{
	enum Type
	{
		None,
		Whitespace,
		Identifier,
		Number,
		Comment,

	};
}

class HLSLTokenizer
{
public:


	HLSLTokenizer(void);
	~HLSLTokenizer(void);


private:
};

