#include "HLSLExpressionNode.h"
#include "HLSLNodeVisitor.h"

HLSLExpressionNode::HLSLExpressionNode(const HLSLToken& token) :
	HLSLStatementNode(token)
{
}


HLSLExpressionNode::~HLSLExpressionNode(void)
{
}


HLSLIdentifierNode::HLSLIdentifierNode(const HLSLToken& token) :
	HLSLExpressionNode(token)
{

}

const std::string& HLSLIdentifierNode::GetName() const
{
	return mToken.GetValue();
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