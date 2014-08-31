#pragma once

#include <string>
#include <cstdlib>

namespace HLSLTokenType
{
	enum Type
	{
		None,
		Error,
		EndOfFile,
		Whitespace,
		Identifier,
		Number,
		String,
		Comment,
		
		Add,
		Minus,
		Multiply,
		Divide,
		Modulus,

		Assign,
		AddEqual,
		MinusEqual,
		MultiplyEqual,
		DivideEqual,
		ModulusEqual,

		BitwiseNot,
		LeftShift,
		RightShift,
		BitwiseAnd,
		BitwiseOr,
		BitwiseXor,
		LeftShiftEqual,
		RightShiftEqual,
		BitwiseAndEqual,
		BitwiseOrEqual,
		BitwiseXorEqual,

		BooleanAnd,
		BooleanOr,

		TernaryOperator,
		Colon,
		SemiColon,

		Comma,

		LessThan,
		GreaterThan,
		Equal,
		NotEqual,
		LessThanEqual,
		GreaterThanEqual,

		PlusPlus,
		MinusMinus,

		StructureOperator,

		BooleanNot,

		OpenSquare,
		CloseSquare,

		OpenParen,
		CloseParen,

		OpenCurly,
		CloseCurly
	};
}

class HLSLToken
{
public:
	HLSLToken(HLSLTokenType::Type type, const std::string& value, size_t lineNumber);
	~HLSLToken(void);

	bool IsBinaryOperator() const;
	bool IsUnaryOperator() const;
	
	size_t GetLineNumber() const;
	const std::string& GetValue() const;

	bool operator==(const HLSLToken& other) const;
private:
	HLSLTokenType::Type mType;
	std::string mValue;
	size_t mLineNumber;
};

