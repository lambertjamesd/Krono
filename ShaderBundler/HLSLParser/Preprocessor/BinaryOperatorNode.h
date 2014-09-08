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
	BinaryOperatorNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);
	
	std::unique_ptr<ExpressionNode> mOperandA;
	std::unique_ptr<ExpressionNode> mOperandB;
};

class AddNode : public BinaryOperatorNode
{
public:
	AddNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class SubtractNode : public BinaryOperatorNode
{
public:
	SubtractNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class MultiplyNode : public BinaryOperatorNode
{
public:
	MultiplyNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class DivideNode : public BinaryOperatorNode
{
public:
	DivideNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class ModulusNode : public BinaryOperatorNode
{
public:
	ModulusNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class EqualNode : public BinaryOperatorNode
{
public:
	EqualNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class NotEqualNode : public BinaryOperatorNode
{
public:
	NotEqualNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class GreaterThanNode : public BinaryOperatorNode
{
public:
	GreaterThanNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class LessThanNode : public BinaryOperatorNode
{
public:
	LessThanNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class GreaterThanEqualNode : public BinaryOperatorNode
{
public:
	GreaterThanEqualNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class LessThanEqualNode : public BinaryOperatorNode
{
public:
	LessThanEqualNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class BooleanAndNode : public BinaryOperatorNode
{
public:
	BooleanAndNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class BooleanOrNode : public BinaryOperatorNode
{
public:
	BooleanOrNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class BitwiseAndNode : public BinaryOperatorNode
{
public:
	BitwiseAndNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class BitwiseOrNode : public BinaryOperatorNode
{
public:
	BitwiseOrNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class BitwiseXorNode : public BinaryOperatorNode
{
public:
	BitwiseXorNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class BitshiftLeftNode : public BinaryOperatorNode
{
public:
	BitshiftLeftNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

class BitshiftRightNode : public BinaryOperatorNode
{
public:
	BitshiftRightNode(const Token& token, std::unique_ptr<ExpressionNode> operandA, std::unique_ptr<ExpressionNode> operandB);

	virtual void Accept(NodeVisitor& visitor);
private:
};

}