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
		Function,

		// can be a scalar, vector,
		// or matrix of any size
		Numerical,
		// can be a vector of any size
		VariableVector,
	};

	enum ScalarType
	{
		NoScalarType,
		Bool,
		Int,
		UInt,
		DWord,
		Half,
		Float,
		Double,

		// can be any type
		BoolIntFloat,

		// can by any type but bool
		IntFloat,
	};

	enum TextureType
	{
		Texture1D,
		Texture1DArray,
		Texture2D,
		Texture2DArray,
		Texture3D,
		TextureCube,

		TextureTypeCount
	};

	enum Modifier
	{
		NoModifier,
		ArrayModifier,
		TypeClassModifier,
	};

	HLSLType();
	HLSLType(Type type);
	HLSLType(Type type, ScalarType scalarType);
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
	HLSLType GetReturnType() const;
	
	HLSLType ChangeScalarType(ScalarType type) const;

	Type GetType() const;
	ScalarType GetScalarType() const;
	TextureType GetTextureType() const;
	HLSLStructDefinition& GetStructure() const;
	HLSLFunctionDefinition& GetFunction() const;

	void ResolveFunctionOverload(HLSLFunctionDefinition& newValue) const;

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

	bool NeedsTypeResolution() const;

	// if target is ambigious in any way then this method resolves those
	// ambiguities using input
	bool ResolveAmbigiousType(HLSLType& target, const HLSLType& input) const;
	
	static bool TypeNeedsResolution(Type type);
	static bool ScalarTypeNeedsResolution(ScalarType scalarType);
	static ScalarType MorePowerfulScalar(ScalarType a, ScalarType b);
	
	static const size_t NonNumerical = ~0;
	static const size_t MaxVectorSize = 4;
	
	static const size_t MaxMatrixRows = 4;
	static const size_t MaxMatrixColumns = 4;
private:
	Type mType;
	Modifier mModifier;
	
	void SetVectorSize(size_t value);
	void SetMatrixSize(size_t rows, size_t columns);

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
