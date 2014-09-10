#pragma once

#include <string>
#include <vector>

class HLSLStructDefinition;
class HLSLFunctionDefinition;

class HLSLType;

class HLSLFunctionInputSignature
{
public:
	HLSLFunctionInputSignature();
	HLSLFunctionInputSignature(const std::vector<HLSLType>& typeList);

	size_t GetParameterCount() const;
	HLSLType GetParameter(size_t index) const;
private:
	std::vector<HLSLType> mTypeList;
};


class HLSLType
{
public:
	enum Type
	{
		Unknown,
		Void,
		Scalar,
		Vector,
		Matrix,
		Texture,
		Sampler,
		Struct,
		Function
	};

	enum ScalarType
	{
		Bool,
		Int,
		UInt,
		DWord,
		Half,
		Float,
		Double,
	};

	enum TextureType
	{
		Texture1D,
		Texture1DArray,
		Texture2D,
		Texture2DArray,
		Texture3D,
		TextureCube
	};

	enum Modifier
	{
		NoModifier,
		ArrayModifier,
		TypeClassModifier
	};

	HLSLType();
	HLSLType(Type type);
	HLSLType(ScalarType scalarType);
	HLSLType(ScalarType scalarType, size_t vectorSize);
	HLSLType(ScalarType scalarType, size_t rows, size_t columns);
	
	HLSLType(TextureType textureType);

	HLSLType(HLSLStructDefinition& structDefinition);
	HLSLType(HLSLFunctionDefinition& functionDefinition);

	HLSLType BoolType() const;

	// variable size array type
	HLSLType ArrayType() const;

	HLSLType ArrayType(size_t count) const;

	HLSLType TypeClass() const;
	HLSLType InstanceType() const;

	HLSLType VectorType(size_t count) const;
	HLSLType MatrixType(size_t rows, size_t columns) const;

	HLSLType ArrayElementType() const;
	HLSLType SubElement(const std::string& name) const;
	HLSLType ResolveReturnType(const HLSLFunctionInputSignature& inputSignature) const;
	HLSLType GetReturnType() const;

	Type GetType() const;
	ScalarType GetScalarType() const;
	TextureType GetTextureType() const;
	HLSLStructDefinition& GetStructure() const;
	HLSLFunctionDefinition& GetFunction() const;

	bool IsInteger() const;
	bool IsNumerical() const;
	bool IsSingleValue() const;
	bool IsIndexable() const;
	bool IsArray() const;
	bool IsTypeClass() const;

	size_t GetArraySize() const;
	size_t GetVectorSize() const;
	size_t GetRows() const;
	size_t GetColumns() const;

	bool StrictlyEqual(const HLSLType& other) const;
	bool CanAssignFromLossless(const HLSLType& other) const;
	bool CanAssignFrom(const HLSLType& other) const;
	
	size_t GetScalarCount() const;
	bool IsPureNumerical() const;
	
	static const size_t NonNumerical = ~0;
private:
	Type mType;
	Modifier mModifier;

	static const size_t gSizeMask = 0x0FFFFFFF;
	static const size_t gRowMask = 0x30000000;
	static const size_t gColMask = 0xC0000000;

	union
	{
		ScalarType mScalarType;
		TextureType mTextureType;
		HLSLStructDefinition* mStructDefinition;
		HLSLFunctionDefinition* mFunctionDefinition;

	};

	size_t mSize;
};
