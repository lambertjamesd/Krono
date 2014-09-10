#include "HLSLBinaryOperatorNode.h"
#include "HLSLNodeVisitor.h"

HLSLBinaryOperatorNode::HLSLBinaryOperatorNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> left, std::unique_ptr<HLSLExpressionNode> right) :
	HLSLExpressionNode(token),

	mLeft(move(left)),
	mRight(move(right))
{

}

HLSLBinaryOperatorNode::~HLSLBinaryOperatorNode(void)
{

}

HLSLExpressionNode& HLSLBinaryOperatorNode::GetLeft()
{
	return *mLeft;
}

HLSLExpressionNode& HLSLBinaryOperatorNode::GetRight()
{
	return *mRight;
}

void HLSLBinaryOperatorNode::SetLeft(std::unique_ptr<HLSLExpressionNode> value)
{
	mLeft = move(value);
}

void HLSLBinaryOperatorNode::SetRight(std::unique_ptr<HLSLExpressionNode> value)
{
	mRight = move(value);
}

std::unique_ptr<HLSLExpressionNode> HLSLBinaryOperatorNode::TakeLeft()
{
	return move(mLeft);
}

std::unique_ptr<HLSLExpressionNode> HLSLBinaryOperatorNode::TakeRight()
{
	return move(mRight);
}

std::unique_ptr<HLSLBinaryOperatorNode> HLSLBinaryOperatorNode::CreateNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> left, std::unique_ptr<HLSLExpressionNode> right)
{
	return std::unique_ptr<HLSLBinaryOperatorNode>(new HLSLBinaryOperatorNode(token, move(left), move(right)));
}

void HLSLBinaryOperatorNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}