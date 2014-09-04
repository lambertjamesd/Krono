#include "UnaryOperatorNode.h"
#include "NodeVisitor.h"

namespace preproc
{

UnaryOperatorNode::UnaryOperatorNode(std::unique_ptr<ExpressionNode> internalValue) :
	mInternalValue(move(internalValue))
{
}


UnaryOperatorNode::~UnaryOperatorNode(void)
{
}

ExpressionNode& UnaryOperatorNode::GetInternalValue()
{
	return *mInternalValue;
}

NegateOperatorNode::NegateOperatorNode(std::unique_ptr<ExpressionNode> internalValue) :
	UnaryOperatorNode(move(internalValue))
{

}

void NegateOperatorNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BitwiseNotOperatorNode::BitwiseNotOperatorNode(std::unique_ptr<ExpressionNode> internalValue) :
	UnaryOperatorNode(move(internalValue))
{

}

void BitwiseNotOperatorNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BooleanNotOperatorNode::BooleanNotOperatorNode(std::unique_ptr<ExpressionNode> internalValue) :
	UnaryOperatorNode(move(internalValue))
{

}

void BooleanNotOperatorNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

}