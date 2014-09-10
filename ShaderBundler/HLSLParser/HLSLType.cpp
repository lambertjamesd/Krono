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

size_t HLSLFunctionInputSignature::GetParameterCount() const
{
	return mTypeList.size();
}

HLSLType HLSLFunctionInputSignature::GetParameter(size_t index) const
{
	return mTypeList[index];
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


HLSLType HLSLType::BoolType() const
{
	assert(mModifier == NoModifier && "only numerical or object types have boolean values");

	switch (mType)
	{
	case Scalar:
	case Texture:
	case Sampler:
		return HLSLType(HLSLType::Bool);
	case Vector:
		return HLSLType(HLSLType::Bool, GetVectorSize());
	case Matrix:
		return HLSLType(HLSLType::Bool, GetRows(), GetColumns());
	}

	return HLSLType(HLSLType::Unknown);
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

HLSLType HLSLType::ResolveReturnType(const HLSLFunctionInputSignature& inputSignature) const
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

		HLSLFunctionDefinition* exactMatch = NULL;
		HLSLFunctionDefinition* compatableMatch = NULL;
		bool isAbigious = false;

		HLSLFunctionDefinition* looper = mFunctionDefinition;

		while (looper != NULL)
		{
			if (looper->Matches(inputSignature))
			{
				if (exactMatch != NULL)
				{
					return HLSLType::Unknown;
				}
				else
				{
					exactMatch = looper;
				}
			}
			else if (looper->IsCompatibleWith(inputSignature))
			{
				if (compatableMatch != NULL)
				{
					isAbigious = true;
				}
				else
				{
					compatableMatch = looper;
				}
			}

			looper = looper->GetPreviousOverload();
		}

		if (exactMatch != NULL)
		{
			const_cast<HLSLFunctionDefinition*>(mFunctionDefinition) = exactMatch;
			return exactMatch->GetReturnType().GetType();
		}
		else if (compatableMatch != NULL)
		{
			if (isAbigious)
			{
				return HLSLType::Unknown;
			}
			else
			{
				const_cast<HLSLFunctionDefinition*>(mFunctionDefinition) = compatableMatch;
				return compatableMatch->GetReturnType().GetType();
			}
		}
		else
		{
			return HLSLType::Void;
		}
	}
}

HLSLType HLSLType::GetReturnType() const
{
	assert(mType == Function);
	return mFunctionDefinition->GetReturnType().GetType();
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

HLSLStructDefinition& HLSLType::GetStructure() const
{
	assert(mType == Struct);
	return *mStructDefinition;
}

HLSLFunctionDefinition& HLSLType::GetFunction() const
{
	assert(mType == Function);
	return *mFunctionDefinition;
}

bool HLSLType::IsInteger() const
{
	return IsNumerical() && (
		mScalarType == UInt ||
		mScalarType == Int ||
		mScalarType == DWord);
}

bool HLSLType::IsNumerical() const
{
	return mType == Scalar || mType == Vector || mType == Matrix;
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

bool HLSLType::StrictlyEqual(const HLSLType& other) const
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

bool HLSLType::CanAssignFromLossless(const HLSLType& other) const
{
	if (IsArray() && other.IsArray())
	{
		return GetArraySize() == other.GetArraySize() && ArrayElementType().CanAssignFrom(other.ArrayElementType());
	}
	else if (IsArray() != other.IsArray())
	{
		return false;
	}

	if (IsTypeClass() || other.IsTypeClass())
	{
		return false;
	}

	if (IsNumerical() && other.IsNumerical())
	{
		return mScalarType == other.mScalarType && GetScalarCount() == other.GetScalarCount();
	}

	if (mType != other.mType)
	{
		return false;
	}

	else if (mType == Struct)
	{
		if (mStructDefinition->GetMemberCount() != other.mStructDefinition->GetMemberCount())
		{
			return false;
		}

		for (size_t i = 0; i < mStructDefinition->GetMemberCount(); ++i)
		{
			if (!mStructDefinition->GetMember(i).GetType().GetType().CanAssignFromLossless(
				other.mStructDefinition->GetMember(i).GetType().GetType()))
			{
				return false;
			}
		}

		return true;
	}
	else if (mType == Texture)
	{
		return mTextureType == other.mTextureType;
	}
	else if (mType == Sampler)
	{
		return true;
	}

	return false;
}

bool HLSLType::CanAssignFrom(const HLSLType& other) const
{
	if (IsNumerical() && mModifier == NoModifier && 
		other.IsNumerical() && other.mModifier == NoModifier)
	{
		if (mType == other.mType)
		{
			if (mType == Scalar)
			{
				return true;
			}
			else if (mType == Vector)
			{
				return GetVectorSize() <= other.GetVectorSize();
			}
			else if (mType == Matrix)
			{
				return GetRows() <= other.GetRows() && GetColumns() <= other.GetColumns();
			}
			else
			{
				assert(false && "Invalid numerical type");
				return false;
			}
		}
		else
		{
			return GetScalarCount() == other.GetScalarCount();
		}
	}
	else
	{
		return CanAssignFromLossless(other);
	}
}

size_t HLSLType::GetScalarCount() const
{
	if (mModifier == NoModifier)
	{
		switch (mType)
		{
		case Scalar:
			return 1;
		case Vector:
			return GetVectorSize();
		case Matrix:
			return GetRows() * GetColumns();
		case Struct:
			{
				size_t result = 0;

				for (size_t i = 0; i < mStructDefinition->GetMemberCount(); ++i)
				{
					size_t memberSize = mStructDefinition->GetMember(i).GetType().GetType().GetScalarCount();

					if (memberSize == NonNumerical)
					{
						return NonNumerical;
					}
					else
					{
						result += memberSize;
					}
				}

				return result;
			}
		}
	}
	
	return NonNumerical;
}

bool HLSLType::IsPureNumerical() const
{
	if (mModifier != NoModifier)
	{
		return false;
	}

	switch (mType)
	{
	case Void:
	case Texture:
	case Sampler:
	case Function:
		return false;
	case Scalar:
	case Vector:
	case Matrix:
		return true;
	case Struct:
		{
			for (size_t i = 0; i < mStructDefinition->GetMemberCount(); ++i)
			{
				if (!mStructDefinition->GetMember(i).GetType().GetType().IsPureNumerical())
				{
					return false;
				}
			}

			return true;
		}
	default:
		assert(false && "invalid type");
	}

	return false;
}