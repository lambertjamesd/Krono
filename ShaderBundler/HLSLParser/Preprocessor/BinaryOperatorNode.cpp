#include "BinaryOperatorNode.h"
#include "NodeVisitor.h"


namespace preproc
{

BinaryOperatorNode::BinaryOperatorNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	ExpressionNode(token),
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

AddNode::AddNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void AddNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

SubtractNode::SubtractNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void SubtractNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

MultiplyNode::MultiplyNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void MultiplyNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

DivideNode::DivideNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void DivideNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

ModulusNode::ModulusNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void ModulusNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

EqualNode::EqualNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void EqualNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

NotEqualNode::NotEqualNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void NotEqualNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

GreaterThanNode::GreaterThanNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void GreaterThanNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

LessThanNode::LessThanNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void LessThanNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

GreaterThanEqualNode::GreaterThanEqualNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void GreaterThanEqualNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

LessThanEqualNode::LessThanEqualNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void LessThanEqualNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BooleanAndNode::BooleanAndNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void BooleanAndNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BooleanOrNode::BooleanOrNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void BooleanOrNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BitwiseAndNode::BitwiseAndNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void BitwiseAndNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BitwiseOrNode::BitwiseOrNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void BitwiseOrNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BitwiseXorNode::BitwiseXorNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void BitwiseXorNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BitshiftLeftNode::BitshiftLeftNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void BitshiftLeftNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

BitshiftRightNode::BitshiftRightNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB) :
	BinaryOperatorNode(token, move(operandA), move(operandB))
{

}

void BitshiftRightNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

}