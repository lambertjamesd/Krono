#pragma once

#include "Node.h"

namespace preproc
{

class ExpressionNode : public Node
{
public:
	virtual ~ExpressionNode(void);
protected:
	ExpressionNode(void);
};

class DefinedOperatorNode : public ExpressionNode
{
public:
	DefinedOperatorNode(const std::string& name);
	~DefinedOperatorNode(void);

	virtual void Accept(NodeVisitor& visitor);

	const std::string& GetName();
private:
	std::string mName;
};

class IdentifierNode : public ExpressionNode
{
public:
	IdentifierNode(const std::string& value);
	virtual void Accept(NodeVisitor& visitor);

	const std::string& GetValue() const;
private:
	std::string mValue;
};

class FunctionNode : public ExpressionNode
{
public:
	FunctionNode(const std::string& name);
	virtual void Accept(NodeVisitor& visitor);

	void AddParameter(std::unique_ptr<Node> value);

	const std::string& GetName() const;

	size_t GetParameterCount() const;
	Node* GetParameter(size_t index);
private:
	std::string mName;
	std::vector<std::unique_ptr<Node> > mParameters;
};

class ConstantNode : public ExpressionNode
{
public:
	ConstantNode(const std::string& value);
	virtual void Accept(NodeVisitor& visitor);
	
	const std::string& GetValue() const;
	long GetNumberValue() const;
private:
	std::string mValue;
	long mNumberValue;
};

}