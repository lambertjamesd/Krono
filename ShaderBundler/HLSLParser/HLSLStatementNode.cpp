#include "HLSLStatementNode.h"
#include "HLSLNodeVisitor.h"
#include "HLSLExpressionNode.h"

#include <iostream>
#include <cassert>

HLSLStatementNode::HLSLStatementNode(const HLSLToken& token) :
	HLSLNode(token)
{
}


HLSLStatementNode::~HLSLStatementNode(void)
{
}

HLSLStatementBlock::HLSLStatementBlock(void)
{

}

void HLSLStatementBlock::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLTypedefDefinition::HLSLTypedefDefinition(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> type, const std::string& name) :
	HLSLStatementNode(token),
	mType(move(type)),
	mName(name)
{

}

const std::shared_ptr<HLSLTypeNode>& HLSLTypedefDefinition::GetType() const
{
	return mType;
}

const std::string& HLSLTypedefDefinition::GetName() const
{
	return mName;
}

void HLSLTypedefDefinition::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLRegisterLocation::HLSLRegisterLocation() :
	mRegisterNumber(0),
	mIndex(0)
{

}

HLSLRegisterLocation::HLSLRegisterLocation(const std::string& name, size_t registerNumber, size_t index) :
	mName(name),
	mRegisterNumber(registerNumber),
	mIndex(index)
{

}

bool HLSLRegisterLocation::HasValue() const
{
	return mName.length() > 0;
}

const std::string& HLSLRegisterLocation::GetName() const
{
	return mName;
}

size_t HLSLRegisterLocation::GetRegisterNumber() const
{
	return mRegisterNumber;
}

size_t HLSLRegisterLocation::GetIndex() const
{
	return mIndex;
}

HLSLVariableDefinition::HLSLVariableDefinition(const HLSLToken& token, 
		VariableStorageClass::Type storageClass, 
		VariableTypeModifer::Type typeModifier, 
		std::unique_ptr<HLSLTypeNode> type,
		const std::string& name) :
	HLSLStatementNode(token),
	mStorageClass(storageClass),
	mTypeModifier(typeModifier),
	mType(move(type)),
	mName(name)
{

}
	
void HLSLVariableDefinition::SetSemantic(const HLSLSemantic& semantic)
{
	mSemantic = semantic;
}

void HLSLVariableDefinition::SetRegisterLocation(const HLSLRegisterLocation& value)
{
	mRegisterLocation = value;
}

void HLSLVariableDefinition::SetInitialValue(std::unique_ptr<HLSLExpressionNode> value)
{
	mInitialValue = move(value);
}

VariableStorageClass::Type HLSLVariableDefinition::GetStorageClass() const
{
	return mStorageClass;
}

VariableTypeModifer::Type HLSLVariableDefinition::GetTypeModifier() const
{
	return mTypeModifier;
}

HLSLTypeNode& HLSLVariableDefinition::GetType()
{
	return *mType;
}

const std::shared_ptr<HLSLTypeNode>& HLSLVariableDefinition::GetTypePtr() const
{
	return mType;
}

const std::string& HLSLVariableDefinition::GetName() const
{
	return mName;
}

const HLSLSemantic& HLSLVariableDefinition::GetSemantic() const
{
	return mSemantic;
}

const HLSLRegisterLocation& HLSLVariableDefinition::GetRegisterLocation() const
{
	return mRegisterLocation;
}

HLSLExpressionNode* HLSLVariableDefinition::GetInitialValue()
{
	return mInitialValue.get();
}

bool HLSLVariableDefinition::HasInitialValue() const
{
	return mInitialValue != NULL;
}

void HLSLVariableDefinition::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLSemantic::HLSLSemantic()
{

}

HLSLSemantic::HLSLSemantic(const std::string& value) :
	mSemantic(value)
{

}

const std::string& HLSLSemantic::GetValue() const
{
	return mSemantic;
}

HLSLFunctionParameter::HLSLFunctionParameter(const HLSLToken& token, InputModifier inputModifer, std::unique_ptr<HLSLTypeNode> type, const std::string& name) :
	HLSLNode(token),
	mInputModifier(inputModifer),
	mInterpolationMode(InterpolationMode::None),
	mVariableDefinition(token, VariableStorageClass::None, VariableTypeModifer::None, move(type), name)

{

}

void HLSLFunctionParameter::SetSemantic(const HLSLSemantic& value)
{
	mVariableDefinition.SetSemantic(value);
}

void HLSLFunctionParameter::SetInterpolationMode(InterpolationMode::Type value)
{
	mInterpolationMode = value;
}

void HLSLFunctionParameter::SetInitialzier(std::unique_ptr<HLSLExpressionNode> value)
{
	mVariableDefinition.SetInitialValue(move(value));
}

HLSLFunctionParameter::InputModifier HLSLFunctionParameter::GetInputModifier() const
{
	return mInputModifier;
}

HLSLTypeNode& HLSLFunctionParameter::GetType()
{
	return mVariableDefinition.GetType();
}
const std::string& HLSLFunctionParameter::GetName() const
{
	return mVariableDefinition.GetName();
}

const HLSLSemantic& HLSLFunctionParameter::GetSemantic() const
{
	return mVariableDefinition.GetSemantic();
}

InterpolationMode::Type HLSLFunctionParameter::GetInterpolationMode() const
{
	return mInterpolationMode;
}

HLSLExpressionNode* HLSLFunctionParameter::GetInitializer()
{
	return mVariableDefinition.GetInitialValue();
}

bool HLSLFunctionParameter::IsOptional() const
{
	return mVariableDefinition.HasInitialValue();
}

HLSLVariableDefinition& HLSLFunctionParameter::GetVariableDefinition()
{
	return mVariableDefinition;
}

void HLSLFunctionParameter::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLFunctionDefinition::HLSLFunctionDefinition(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> returnType, const std::string& name) :
	HLSLStatementNode(token),
	mPreviousOverloadedDefinition(NULL),
	mReturnType(move(returnType)),
	mName(name)
{

}

void HLSLFunctionDefinition::AddParameter(std::unique_ptr<HLSLFunctionParameter> value)
{
	mParameters.push_back(move(value));
}

void HLSLFunctionDefinition::RemoveParameter(int index)
{
	for (size_t i = index + 1; i < mParameters.size(); ++i)
	{
		mParameters[i - 1] = move(mParameters[i]);
	}

	mParameters.pop_back();
}

void HLSLFunctionDefinition::ReplaceParameter(int index, std::unique_ptr<HLSLFunctionParameter> value)
{
	mParameters[index] = move(value);
}

void HLSLFunctionDefinition::SetSemantic(const HLSLSemantic& value)
{
	mSemantic = value;
}

const HLSLSemantic& HLSLFunctionDefinition::GetSemantic() const
{
	return mSemantic;
}

void HLSLFunctionDefinition::SetBody(std::unique_ptr<HLSLStatementBlock> value)
{
	mBody = move(value);
}

HLSLTypeNode& HLSLFunctionDefinition::GetReturnType()
{
	return *mReturnType;
}

const HLSLTypeNode& HLSLFunctionDefinition::GetReturnType() const
{
	return *mReturnType;
}

// resolves the return type based on the parameters
bool HLSLFunctionDefinition::ResolveReturnType(const HLSLFunctionInputSignature& parameters, HLSLType& result) const
{
	assert(IsCompatibleWith(parameters) && "function must be compatable");

	HLSLType returnType = mReturnType->GetType();

	if (returnType.NeedsTypeResolution())
	{
		HLSLType resolvedType(HLSLType::Numerical, HLSLType::NoScalarType);
		
		for (size_t i = 0; i < parameters.GetParameterCount(); ++i)
		{
			HLSLType parameterInputType = mParameters[i]->GetType().GetType();

			if (parameterInputType.NeedsTypeResolution())
			{
				if (!parameterInputType.ResolveAmbigiousType(resolvedType, parameters.GetParameter(i)))
				{
					return false;
				}
			}
		}

		HLSLType returnTypeCopy(returnType);
		if (!returnTypeCopy.ResolveAmbigiousType(returnType, resolvedType))
		{
			return false;
		}
	}
	
	result = returnType;
	return true;
}

const std::string& HLSLFunctionDefinition::GetName() const
{
	return mName;
}

size_t HLSLFunctionDefinition::GetParameterCount() const
{
	return mParameters.size();
}

HLSLFunctionParameter& HLSLFunctionDefinition::GetParameter(size_t index)
{
	return *mParameters[index];
}

HLSLStatementBlock* HLSLFunctionDefinition::GetBody()
{
	return mBody.get();
}

bool HLSLFunctionDefinition::IsOverload(HLSLFunctionDefinition& value)
{
	if (&value == this)
	{
		return true;
	}
	else if (mPreviousOverloadedDefinition == NULL)
	{
		return false;
	}
	else
	{
		return mPreviousOverloadedDefinition->IsOverload(value);
	}
}

void HLSLFunctionDefinition::SetPreviousOverload(HLSLFunctionDefinition* value)
{
	mPreviousOverloadedDefinition = value;
}

HLSLFunctionDefinition* HLSLFunctionDefinition::GetPreviousOverload()
{
	return mPreviousOverloadedDefinition;
}

bool HLSLFunctionDefinition::IsCompatibleWith(const HLSLFunctionInputSignature& parameters) const
{
	if (mParameters.size() < parameters.GetParameterCount())
	{
		return false;
	}

	for (size_t i = 0; i < mParameters.size(); ++i)
	{
		if (i < parameters.GetParameterCount())
		{
			if (!mParameters[i]->GetType().GetType().CanAssignFrom(parameters.GetParameter(i)))
			{
				return false;
			}
		}
		else if (!mParameters[i]->IsOptional())
		{
			return false;
		}
	}

	return true;
}

bool HLSLFunctionDefinition::Matches(const HLSLFunctionInputSignature& parameters) const
{
	if (mParameters.size() < parameters.GetParameterCount())
	{
		return false;
	}

	for (size_t i = 0; i < mParameters.size(); ++i)
	{
		if (i < parameters.GetParameterCount())
		{
			if (!mParameters[i]->GetType().GetType().CanAssignFromLossless(parameters.GetParameter(i)))
			{
				return false;
			}
		}
		else if (!mParameters[i]->IsOptional())
		{
			return false;
		}
	}

	return true;
}

void HLSLFunctionDefinition::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLReturnStatement::HLSLReturnStatement(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> returnValue) :
	HLSLStatementNode(token),
	mReturnValue(move(returnValue))
{

}

bool HLSLReturnStatement::HasReturnValue() const
{
	return mReturnValue != NULL;
}

HLSLExpressionNode& HLSLReturnStatement::GetReturnValue()
{
	return *mReturnValue;
}
	
void HLSLReturnStatement::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLStructureMember::HLSLStructureMember(const HLSLToken& token, InterpolationMode::Type interpolationMode, std::unique_ptr<HLSLTypeNode> type, const std::string& name) :
	HLSLNode(token),
	mInterpolationMode(interpolationMode),
	mType(move(type)),
	mName(name)
{

}

void HLSLStructureMember::SetSemantic(const HLSLSemantic& value)
{
	mSemantic = value;
}

InterpolationMode::Type HLSLStructureMember::GetInterpolationMode() const
{
	return mInterpolationMode;
}


const HLSLTypeNode& HLSLStructureMember::GetType() const
{
	return *mType;
}

HLSLTypeNode& HLSLStructureMember::GetType()
{
	return *mType;
}

const std::string& HLSLStructureMember::GetName()
{
	return mName;
}

const HLSLSemantic& HLSLStructureMember::GetSemantic()
{
	return mSemantic;
}

void HLSLStructureMember::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLStructDefinition::HLSLStructDefinition(const HLSLToken& token, const std::string& name) :
	HLSLStatementNode(token),
	mName(name)
{

}

void HLSLStructDefinition::AddMember(std::unique_ptr<HLSLStructureMember> member)
{
	mMemberList.push_back(move(member));
}

void HLSLStructDefinition::RemoveMember(size_t index)
{
	for (size_t i = index + 1; i < mMemberList.size(); ++i)
	{
		mMemberList[i - 1] = move(mMemberList[i]);
	}

	mMemberList.pop_back();
}

void HLSLStructDefinition::ReplaceMember(size_t index, std::unique_ptr<HLSLStructureMember> member)
{
	mMemberList[index] = move(member);
}

const std::string& HLSLStructDefinition::GetName() const
{
	return mName;
}

size_t HLSLStructDefinition::GetMemberCount() const
{
	return mMemberList.size();
}

const HLSLStructureMember& HLSLStructDefinition::GetMember(size_t index) const
{
	return *mMemberList[index];
}

HLSLStructureMember& HLSLStructDefinition::GetMember(size_t index)
{
	return *mMemberList[index];
}

const HLSLStructureMember* HLSLStructDefinition::GetMemberByName(const std::string& name) const
{
	for (auto it = mMemberList.begin(); it != mMemberList.end(); ++it)
	{
		if ((*it)->GetName() == name)
		{
			return it->get();
		}
	}

	return NULL;
}

void HLSLStructDefinition::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLCBufferDefinition::HLSLCBufferDefinition(const HLSLToken& token, const std::string& name) :
	HLSLStatementNode(token),
	mName(name)
{

}

void HLSLCBufferDefinition::SetRegister(const HLSLRegisterLocation& registerLocation)
{
	mRegisterLocation = registerLocation;
}

void HLSLCBufferDefinition::AddMember(std::unique_ptr<HLSLVariableDefinition> member)
{
	mMemberList.push_back(move(member));
}

const std::string& HLSLCBufferDefinition::GetName() const
{
	return mName;
}

const HLSLRegisterLocation& HLSLCBufferDefinition::GetRegisterLocation() const
{
	return mRegisterLocation;
}
	
size_t HLSLCBufferDefinition::GetMemberCount() const
{
	return mMemberList.size();
}

HLSLVariableDefinition& HLSLCBufferDefinition::GetMember(size_t index)
{
	return *mMemberList[index];
}

void HLSLCBufferDefinition::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}
