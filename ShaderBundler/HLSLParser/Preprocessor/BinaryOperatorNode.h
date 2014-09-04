#pragma once

#include "ExpressionNode.h"

namespace preproc
{

class BinaryOperatorNode : public ExpressionNode
{
public:
	~BinaryOperatorNode(void);

	ExpressionNode& GetOperandA();
	ExpressionNode& GetOperandB();
protected:
	BinaryOperatorNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);
	
	std::unique_ptr<ExpressionNode> mOperandA;
	std::unique_ptr<ExpressionNode> mOperandB;
};

class AddNode : public BinaryOperatorNode
{
public:
	AddNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class SubtractNode : public BinaryOperatorNode
{
public:
	SubtractNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class MultiplyNode : public BinaryOperatorNode
{
public:
	MultiplyNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class DivideNode : public BinaryOperatorNode
{
public:
	DivideNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class ModulusNode : public BinaryOperatorNode
{
public:
	ModulusNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class EqualNode : public BinaryOperatorNode
{
public:
	EqualNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class NotEqualNode : public BinaryOperatorNode
{
public:
	NotEqualNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class GreaterThanNode : public BinaryOperatorNode
{
public:
	GreaterThanNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class LessThanNode : public BinaryOperatorNode
{
public:
	LessThanNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class GreaterThanEqualNode : public BinaryOperatorNode
{
public:
	GreaterThanEqualNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class LessThanEqualNode : public BinaryOperatorNode
{
public:
	LessThanEqualNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class BooleanAndNode : public BinaryOperatorNode
{
public:
	BooleanAndNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class BooleanOrNode : public BinaryOperatorNode
{
public:
	BooleanOrNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class BitwiseAndNode : public BinaryOperatorNode
{
public:
	BitwiseAndNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class BitwiseOrNode : public BinaryOperatorNode
{
public:
	BitwiseOrNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class BitwiseXorNode : public BinaryOperatorNode
{
public:
	BitwiseXorNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class BitshiftLeftNode : public BinaryOperatorNode
{
public:
	BitshiftLeftNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class BitshiftRightNode : public BinaryOperatorNode
{
public:
	BitshiftRightNode(std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

}