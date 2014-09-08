#pragma once

#include <string>

class HLSLFunctionSignature;

class HLSLType
{
public:
	virtual ~HLSLType();

	virtual HLSLType* Member(const std::string& name) = 0;
	virtual HLSLType* FunctionCall(const HLSLFunctionSignature& signature);
	virtual HLSLType* Index();
	virtual HLSLType* Instance();
protected:
	HLSLType();
private:
};

class HLSLScalarType : public HLSLType
{
public:
};

class HLSLVectorType : public HLSLType
{
public:
};

class HLSLMatrixType : public HLSLType
{
public:
};

class HLSLArrayType : public HLSLType
{
public:
};
