#include "HLSLPrefixNode.h"
#include "HLSLNodeVisitor.h"

HLSLPrefixNode::HLSLPrefixNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> right) :
	HLSLExpressionNode(token),
	mRight(move(right))
{
}


HLSLPrefixNode::~HLSLPrefixNode(void)
{
}

HLSLExpressionNode& HLSLPrefixNode::GetRight()
{
	return *mRight;
}

HLSLPrefixIncrNode::HLSLPrefixIncrNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> right) :
	HLSLPrefixNode(token, move(right))
{

}

void HLSLPrefixIncrNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}


HLSLPrefixDecrNode::HLSLPrefixDecrNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> right) :
	HLSLPrefixNode(token, move(right))
{

}

void HLSLPrefixDecrNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}