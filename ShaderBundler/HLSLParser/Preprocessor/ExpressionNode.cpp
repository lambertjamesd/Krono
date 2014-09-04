#include "ExpressionNode.h"
#include "NodeVisitor.h"


namespace preproc
{

ExpressionNode::ExpressionNode(void)
{

}


ExpressionNode::~ExpressionNode(void)
{

}

DefinedOperatorNode::DefinedOperatorNode(const std::string& name, bool useParenthesis) :
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

IdentifierNode::IdentifierNode(const std::string& value) :
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

FunctionNode::FunctionNode(const std::string& name) :
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

ConstantNode::ConstantNode(long value) :
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
