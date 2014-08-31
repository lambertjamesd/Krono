#include "HLSLToken.h"


HLSLToken::HLSLToken(HLSLTokenType::Type type, const std::string& value, size_t lineNumber) :
	mType(type),
	mValue(value),
	mLineNumber(lineNumber)
{

}

HLSLToken::~HLSLToken(void)
{

}

bool HLSLToken::IsBinaryOperator() const
{
	return mType == HLSLTokenType::Add || mType == HLSLTokenType::Minus ||
		mType == HLSLTokenType::Multiply || mType == HLSLTokenType::Divide ||
		mType == HLSLTokenType::Modulus;
}

bool HLSLToken::IsUnaryOperator() const
{
	return mType == HLSLTokenType::Add || mType == HLSLTokenType::Minus ||
		mType == HLSLTokenType::BitwiseNot || mType == HLSLTokenType::BooleanNot;
}
	
size_t HLSLToken::GetLineNumber() const
{
	return mLineNumber;
}

const std::string& HLSLToken::GetValue() const
{
	return mValue;
}

bool HLSLToken::operator==(const HLSLToken& other) const
{
	return mType == other.mType && mValue == other.mValue;
}