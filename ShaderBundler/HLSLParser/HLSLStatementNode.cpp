#include "HLSLStatementNode.h"
#include "HLSLNodeVisitor.h"
#include "HLSLExpressionNode.h"

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
	
void HLSLVariableDefinition::SetSemantic(const std::string& semantic)
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

const std::string& HLSLVariableDefinition::GetSemantic() const
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

void HLSLVariableDefinition::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLFunctionParameter::HLSLFunctionParameter(const HLSLToken& token, InputModifier inputModifer, std::unique_ptr<HLSLTypeNode> type, const std::string& name) :
	HLSLNode(token),
	mInputModifier(inputModifer),
	mType(move(type)),
	mName(name),

	mInterpolationMode(InterpolationMode::None)
{

}

void HLSLFunctionParameter::SetSemantic(const std::string& value)
{
	mSemantic = value;
}

void HLSLFunctionParameter::SetInterpolationMode(InterpolationMode::Type value)
{
	mInterpolationMode = value;
}

void HLSLFunctionParameter::SetInitialzier(std::unique_ptr<HLSLExpressionNode> value)
{
	mInitializer = move(value);
}

HLSLFunctionParameter::InputModifier HLSLFunctionParameter::GetInputModifier() const
{
	return mInputModifier;
}

HLSLTypeNode& HLSLFunctionParameter::GetType()
{
	return *mType;
}
const std::string& HLSLFunctionParameter::GetName() const
{
	return mName;
}

const std::string& HLSLFunctionParameter::GetSemantic() const
{
	return mSemantic;
}

InterpolationMode::Type HLSLFunctionParameter::GetInterpolationMode() const
{
	return mInterpolationMode;
}

HLSLExpressionNode* HLSLFunctionParameter::GetInitializer()
{
	return mInitializer.get();
}

void HLSLFunctionParameter::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLFunctionDefinition::HLSLFunctionDefinition(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> returnType, const std::string& name) :
	HLSLStatementNode(token),
	mReturnType(move(returnType)),
	mName(name)
{

}

void HLSLFunctionDefinition::AddParameter(std::unique_ptr<HLSLFunctionParameter> value)
{
	mParameters.push_back(move(value));
}

void HLSLFunctionDefinition::SetSemantic(const std::string& value)
{
	mSemantic = value;
}

void HLSLFunctionDefinition::SetBody(std::unique_ptr<HLSLStatementBlock> value)
{
	mBody = move(value);
}

HLSLTypeNode& HLSLFunctionDefinition::GetReturnType() const
{
	return *mReturnType;
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

void HLSLFunctionDefinition::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLReturnStatement::HLSLReturnStatement(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> returnValue) :
	HLSLStatementNode(token),
	mReturnValue(move(returnValue))
{

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

void HLSLStructureMember::SetSemantic(const std::string& value)
{
	mSemantic = value;
}

InterpolationMode::Type HLSLStructureMember::GetInterpolationMode() const
{
	return mInterpolationMode;
}

HLSLTypeNode& HLSLStructureMember::GetType()
{
	return *mType;
}

const std::string& HLSLStructureMember::GetName()
{
	return mName;
}

const std::string& HLSLStructureMember::GetSemantic()
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

const std::string& HLSLStructDefinition::GetName() const
{
	return mName;
}

size_t HLSLStructDefinition::GetMemberCount() const
{
	return mMemberList.size();
}

HLSLStructureMember& HLSLStructDefinition::GetMember(size_t index)
{
	return *mMemberList[index];
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
