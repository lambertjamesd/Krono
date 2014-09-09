#include "HLSLType.h"
#include <cassert>
#include "HLSLStatementNode.h"

HLSLFunctionInputSignature::HLSLFunctionInputSignature()
{

}

HLSLFunctionInputSignature::HLSLFunctionInputSignature(const std::vector<HLSLType>& typeList) :
	mTypeList(typeList)
{

}

HLSLType::HLSLType() :
	mType(Unknown),
	mModifier(NoModifier),
	mSize(0)
{

}

HLSLType::HLSLType(Type type) :
	mType(type),
	mModifier(NoModifier),
	mSize(0)
{

}

HLSLType::HLSLType(ScalarType scalarType) :
	mType(Scalar),
	mModifier(NoModifier),
	mScalarType(scalarType),
	mSize(0)
{

}

HLSLType::HLSLType(ScalarType scalarType, size_t vectorSize) :
	mType(Vector),
	mModifier(NoModifier),
	mScalarType(scalarType),
	mSize((vectorSize - 1) << 28)
{
	assert(vectorSize <= 4 && vectorSize > 0);
}

HLSLType::HLSLType(ScalarType scalarType, size_t rows, size_t columns) :
	mType(Matrix),
	mModifier(NoModifier),
	mScalarType(scalarType),
	mSize((rows - 1) << 28 | (columns - 1) << 30)
{
	assert(rows <= 4 && rows > 0);
	assert(columns <= 4 && columns > 0);
}

	
HLSLType::HLSLType(TextureType textureType) :
	mType(Texture),
	mModifier(NoModifier),
	mTextureType(textureType),
	mSize(0)
{

}


HLSLType::HLSLType(HLSLStructDefinition& structDefinition) :
	mType(Struct),
	mModifier(NoModifier),
	mStructDefinition(&structDefinition),
	mSize(0)
{

}

HLSLType::HLSLType(HLSLFunctionDefinition& functionDefinition) :
	mType(Function),
	mModifier(NoModifier),
	mFunctionDefinition(&functionDefinition),
	mSize(0)
{

}


// variable size array type
HLSLType HLSLType::ArrayType() const
{
	return ArrayType(~0);
}


HLSLType HLSLType::ArrayType(size_t count) const
{
	assert(mModifier == NoModifier && "already an array type");
	
	HLSLType result(*this);
	result.mModifier = ArrayModifier;
	result.mSize |= (~0) & gSizeMask;

	return result;
}

HLSLType HLSLType::TypeClass() const
{
	assert(mModifier == NoModifier);

	HLSLType result(*this);
	result.mModifier = TypeClassModifier;
	return result;
}

HLSLType HLSLType::InstanceType() const
{
	assert(mModifier == TypeClassModifier);

	HLSLType result(*this);
	result.mModifier = NoModifier;
	return result;
}

HLSLType HLSLType::VectorType(size_t count) const
{
	assert(mType == Vector && GetVectorSize() == 1);
	return HLSLType(mScalarType, count);
}

HLSLType HLSLType::MatrixType(size_t rows, size_t columns) const
{
	assert(mType == Vector && GetVectorSize() == 1);
	return HLSLType(mScalarType, rows, columns);
}

HLSLType HLSLType::ArrayElementType() const
{
	assert(IsIndexable() && "must be of type array");

	if (mModifier == ArrayModifier)
	{
		HLSLType result(*this);
		result.mModifier = NoModifier;
		result.mSize &= ~gSizeMask;

		return result;
	}
	else if (mType == Matrix)
	{
		return HLSLType(mScalarType, GetColumns());
	}
	else if (mType == Vector)
	{
		return HLSLType(mScalarType);
	}
	else
	{
		assert(false);
		return HLSLType();
	}
}

HLSLType HLSLType::SubElement(const std::string& name) const
{
	assert(mType == Struct);
	const HLSLStructureMember* member = mStructDefinition->GetMemberByName(name);

	if (member == NULL)
	{
		return HLSLType(Void);
	}
	else
	{
		return member->GetType().GetType();
	}
}

HLSLType HLSLType::ReturnType(const HLSLFunctionInputSignature& inputSignature) const
{
	if (mModifier == TypeClassModifier)
	{
		HLSLType result(*this);
		result.mModifier = NoModifier;
		return result;
	}
	else
	{
		assert(mType == Function);
		return mFunctionDefinition->GetReturnType().GetType();
	}
}

HLSLType::Type HLSLType::GetType() const
{
	return mType;
}

HLSLType::ScalarType HLSLType::GetScalarType() const
{
	assert(mType == Scalar || mType == Vector || mType == Matrix);
	return mScalarType;
}

HLSLType::TextureType HLSLType::GetTextureType() const
{
	assert(mType == Texture);
	return mTextureType;
}

bool HLSLType::IsSingleValue() const
{
	return mType == Scalar ||
		(mType == Vector && GetVectorSize() == 1) ||
		(mType == Matrix && GetRows() == 1 && GetColumns() == 1);
}

bool HLSLType::IsIndexable() const
{
	return mModifier == ArrayModifier || mType == Vector || mType == Matrix;;
}

bool HLSLType::IsArray() const
{
	return mModifier == ArrayModifier;
}

bool HLSLType::IsTypeClass() const
{
	return mModifier == TypeClassModifier;
}

size_t HLSLType::GetArraySize() const
{
	assert(mModifier == ArrayModifier);
	return (mSize & gSizeMask) + 1;
}

size_t HLSLType::GetVectorSize() const
{
	assert(mType == Vector);
	return ((mSize & gRowMask) >> 28) + 1;
}

size_t HLSLType::GetRows() const
{
	assert(mType == Matrix);
	return ((mSize & gRowMask) >> 28) + 1;
}

size_t HLSLType::GetColumns() const
{
	assert(mType == Matrix);
	return ((mSize & gColMask) >> 30) + 1;
}

bool HLSLType::StrictlyEqual(const HLSLType& other)
{
	if (mType == other.mType && mModifier == other.mModifier && mSize == other.mSize)
	{
		switch (mType)
		{
		case Unknown:
		case Void:
		case Sampler:
			return true;
		case Scalar:
		case Vector:
		case Matrix:
			return mScalarType == other.mScalarType;
		case Texture:
			return mTextureType == other.mTextureType;
		case Struct:
			return mStructDefinition == other.mStructDefinition;
		case Function:
			return mFunctionDefinition == other.mFunctionDefinition;
		}
	}

	return false;
}

bool HLSLType::CanAssignFrom(const HLSLType& other)
{
	return true;
}
