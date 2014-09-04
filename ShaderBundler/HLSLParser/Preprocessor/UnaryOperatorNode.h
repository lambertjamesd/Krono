#pragma once

#include "ExpressionNode.h"

namespace preproc
{

class UnaryOperatorNode : public ExpressionNode
{
public:
	~UnaryOperatorNode(void);

	ExpressionNode& GetInternalValue();
protected:
	UnaryOperatorNode(std::unique_ptr<ExpressionNode> internalValue);

	std::unique_ptr<ExpressionNode> mInternalValue;
};

class NegateOperatorNode : public UnaryOperatorNode
{
public:
	NegateOperatorNode(std::unique_ptr<ExpressionNode> internalValue);
	
	virtual void Accept(NodeVisitor& visitor);
private:
};

class BitwiseNotOperatorNode : public UnaryOperatorNode
{
public:
	BitwiseNotOperatorNode(std::unique_ptr<ExpressionNode> internalValue);
	
	virtual void Accept(NodeVisitor& visitor);
private:
};

class BooleanNotOperatorNode : public UnaryOperatorNode
{
public:
	BooleanNotOperatorNode(std::unique_ptr<ExpressionNode> internalValue);
	
	virtual void Accept(NodeVisitor& visitor);
private:
};

}