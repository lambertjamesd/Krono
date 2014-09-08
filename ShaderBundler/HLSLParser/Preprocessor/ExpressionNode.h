#pragma once

#include "Node.h"

namespace preproc
{

class ExpressionNode : public Node
{
public:
	virtual ~ExpressionNode(void);
protected:
	ExpressionNode(const Token& token);
};

class DefinedOperatorNode : public ExpressionNode
{
public:
	DefinedOperatorNode(const Token& token, const std::string& name, bool useParenthesis);
	~DefinedOperatorNode(void);

	virtual void Accept(NodeVisitor& visitor);

	const std::string& GetName();
	bool DoesUseParenthesis() const;
private:
	std::string mName;
	bool mUseParenthesis;
};

class IdentifierNode : public ExpressionNode
{
public:
	IdentifierNode(const Token& token);
	IdentifierNode(const Token& token, const std::string& value);
	virtual void Accept(NodeVisitor& visitor);

	const std::string& GetValue() const;
private:
	std::string mValue;
};

class FunctionNode : public ExpressionNode
{
public:
	FunctionNode(const Token& token, const std::string& name);
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
	ConstantNode(const Token& token, long value);
	virtual void Accept(NodeVisitor& visitor);
	
	long GetNumberValue() const;
private:
	long mNumberValue;
};

}
