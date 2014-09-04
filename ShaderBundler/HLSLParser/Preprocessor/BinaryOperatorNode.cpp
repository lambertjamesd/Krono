#include "BinaryOperatorNode.h"
#include "NodeVisitor.h"


namespace preproc
{

BinaryOperatorNode::BinaryOperatorNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	mOperandA(move(operandA)),
	mOperandB(move(operandB))
{

}

BinaryOperatorNode::~BinaryOperatorNode(void)
{

}

ExpressionNode& BinaryOperatorNode::GetOperandA()
{
	return *mOperandA;
}

ExpressionNode& BinaryOperatorNode::GetOperandB()
{
	return *mOperandB;
}

AddNode::AddNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void AddNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

SubtractNode::SubtractNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void SubtractNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

MultiplyNode::MultiplyNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void MultiplyNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

DivideNode::DivideNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void DivideNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

ModulusNode::ModulusNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void ModulusNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

EqualNode::EqualNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void EqualNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

NotEqualNode::NotEqualNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void NotEqualNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

GreaterThanNode::GreaterThanNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void GreaterThanNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

LessThanNode::LessThanNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void LessThanNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

GreaterThanEqualNode::GreaterThanEqualNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void GreaterThanEqualNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

LessThanEqualNode::LessThanEqualNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void LessThanEqualNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BooleanAndNode::BooleanAndNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void BooleanAndNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BooleanOrNode::BooleanOrNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void BooleanOrNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BitwiseAndNode::BitwiseAndNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void BitwiseAndNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BitwiseOrNode::BitwiseOrNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void BitwiseOrNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BitwiseXorNode::BitwiseXorNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void BitwiseXorNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BitshiftLeftNode::BitshiftLeftNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void BitshiftLeftNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BitshiftRightNode::BitshiftRightNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(move(operandA), move(operandB))
{

}

void BitshiftRightNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

}