#include "HLSLExpressionNode.h"
#include "HLSLNodeVisitor.h"
#include <cassert>

HLSLExpressionNode::HLSLExpressionNode(const HLSLToken& token) :
	HLSLStatementNode(token)
{
}

HLSLExpressionNode::~HLSLExpressionNode(void)
{
}

const HLSLType& HLSLExpressionNode::GetType() const
{
	return mType;
}

void HLSLExpressionNode::ResolveType(const HLSLType& type)
{
	assert(mType.GetType() == HLSLType::Unknown);
	mType = type;
}

HLSLIdentifierNode::HLSLIdentifierNode(const HLSLToken& token) :
	HLSLExpressionNode(token),
	mVariableDefintion(NULL)
{

}

const std::string& HLSLIdentifierNode::GetName() const
{
	return mToken.GetValue();
}

bool HLSLIdentifierNode::IsTypeName() const
{
	return mNamedType != NULL;
}

void HLSLIdentifierNode::ResolveNamedType(const std::shared_ptr<HLSLTypeNode>& value)
{
	mNamedType = value;
}

const std::shared_ptr<HLSLTypeNode>& HLSLIdentifierNode::GetNamedType() const
{
	return mNamedType;
}

void HLSLIdentifierNode::ResolveVariableDefinition(HLSLVariableDefinition* variableDefinition)
{
	assert(mVariableDefintion == NULL);
	mVariableDefintion = variableDefinition;
	ResolveType(variableDefinition->GetType().GetType());
}

HLSLVariableDefinition* HLSLIdentifierNode::GetVariableDefinition()
{
	return mVariableDefintion;
}

void HLSLIdentifierNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLNumberConstantNode::HLSLNumberConstantNode(const HLSLToken& token) :
	HLSLExpressionNode(token)
{

}

const std::string& HLSLNumberConstantNode::GetStringValue() const
{
	return mToken.GetValue();
}

void HLSLNumberConstantNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLBooleanConstantNode::HLSLBooleanConstantNode(const HLSLToken& token) :
	HLSLExpressionNode(token)
{

}

bool HLSLBooleanConstantNode::GetBoolValue() const
{
	return mToken.GetKeywordType() == HLSLKeyword::True;
}

void HLSLBooleanConstantNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLTypeExpressionNode::HLSLTypeExpressionNode(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> type) :
	HLSLExpressionNode(token),
	mType(move(type))
{

}

HLSLTypeNode& HLSLTypeExpressionNode::GetType()
{
	return *mType;
}

void HLSLTypeExpressionNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}


HLSLParenthesisNode::HLSLParenthesisNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> value) :
	HLSLExpressionNode(token),
	mValue(move(value))
{

}

HLSLExpressionNode& HLSLParenthesisNode::GetValue()
{
	return *mValue;
}
	
void HLSLParenthesisNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}