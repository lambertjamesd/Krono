#include "ExpressionNode.h"
#include "NodeVisitor.h"


namespace preproc
{

ExpressionNode::ExpressionNode(const Token& token) :
	Node(token)
{

}


ExpressionNode::~ExpressionNode(void)
{

}

DefinedOperatorNode::DefinedOperatorNode(const Token& token, const std::string& name, bool useParenthesis) :
	ExpressionNode(token),
	mName(name),
	mUseParenthesis(useParenthesis)
{

}

DefinedOperatorNode::~DefinedOperatorNode(void)
{

}

void DefinedOperatorNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

const std::string& DefinedOperatorNode::GetName()
{
	return mName;
}

bool DefinedOperatorNode::DoesUseParenthesis() const
{
	return mUseParenthesis;
}

IdentifierNode::IdentifierNode(const Token& token) :
	ExpressionNode(token),
	mValue(token.GetValue())
{

}

IdentifierNode::IdentifierNode(const Token& token, const std::string& value) :
	ExpressionNode(token),
	mValue(value)
{

}

void IdentifierNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

const std::string& IdentifierNode::GetValue() const
{
	return mValue;
}

FunctionNode::FunctionNode(const Token& token, const std::string& name) :
	ExpressionNode(token),
	mName(name)
{

}

void FunctionNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

void FunctionNode::AddParameter(std::unique_ptr<Node> value)
{
	mParameters.push_back(move(value));
}

const std::string& FunctionNode::GetName() const
{
	return mName;
}

size_t FunctionNode::GetParameterCount() const
{
	return mParameters.size();
}

Node* FunctionNode::GetParameter(size_t index)
{
	if (index < mParameters.size())
	{
		return mParameters[index].get();
	}
	else
	{
		return NULL;
	}
}

ConstantNode::ConstantNode(const Token& token, long value) :
	ExpressionNode(token),
	mNumberValue(value)
{

}

void ConstantNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

long ConstantNode::GetNumberValue() const
{
	return mNumberValue;
}

}
