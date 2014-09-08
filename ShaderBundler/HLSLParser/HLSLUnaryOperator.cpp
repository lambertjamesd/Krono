#include "HLSLUnaryOperator.h"
#include "HLSLNodeVisitor.h"

HLSLUnaryOperator::HLSLUnaryOperator(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> value) :
	HLSLExpressionNode(token),
	mInnerExpression(move(value))
{
}


HLSLUnaryOperator::~HLSLUnaryOperator(void)
{
}

HLSLExpressionNode& HLSLUnaryOperator::GetInnerExpression()
{
	return *mInnerExpression;
}

std::unique_ptr<HLSLUnaryOperator> HLSLUnaryOperator::CreateNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> inner)
{
	return std::unique_ptr<HLSLUnaryOperator>(new HLSLUnaryOperator(token, move(inner)));
}

void HLSLUnaryOperator::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLCastNode::HLSLCastNode(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> type, std::unique_ptr<HLSLExpressionNode> right) :
	HLSLExpressionNode(token),
	mType(move(type)),
	mRight(move(right))
{

}

HLSLTypeNode& HLSLCastNode::GetType()
{
	return *mType;
}

HLSLExpressionNode& HLSLCastNode::GetRight()
{
	return *mRight;
}

void HLSLCastNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}