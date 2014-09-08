#include "UnaryOperatorNode.h"
#include "NodeVisitor.h"

namespace preproc
{

UnaryOperatorNode::UnaryOperatorNode(const Token& token, std::unique_ptr<ExpressionNode> internalValue) :
	ExpressionNode(token),
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

NegateOperatorNode::NegateOperatorNode(const Token& token, std::unique_ptr<ExpressionNode> internalValue) :
	UnaryOperatorNode(token, move(internalValue))
{

}

void NegateOperatorNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BitwiseNotOperatorNode::BitwiseNotOperatorNode(const Token& token, std::unique_ptr<ExpressionNode> internalValue) :
	UnaryOperatorNode(token, move(internalValue))
{

}

void BitwiseNotOperatorNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BooleanNotOperatorNode::BooleanNotOperatorNode(const Token& token, std::unique_ptr<ExpressionNode> internalValue) :
	UnaryOperatorNode(token, move(internalValue))
{

}

void BooleanNotOperatorNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

}