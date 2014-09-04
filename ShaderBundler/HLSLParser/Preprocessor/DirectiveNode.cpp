#include "DirectiveNode.h"
#include "NodeVisitor.h"

namespace preproc
{

DefineNode::DefineNode(const std::string& name, const std::vector<std::string>& parameters, bool hasParameters, std::unique_ptr<Node> value, std::unique_ptr<ExpressionNode> expressionValue) :
	mName(name),
	mParameters(mParameters),
	mHasParameters(hasParameters),
	mValue(move(value)),
	mExpressionValue(move(expressionValue))
{

}

void DefineNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

const std::string& DefineNode::GetName() const
{
	return mName;
}

const std::shared_ptr<Node>& DefineNode::GetValue() const
{
	return mValue;
}

const std::shared_ptr<ExpressionNode>& DefineNode::GetExpressionValue() const
{
	return mExpressionValue;
}

bool DefineNode::HasParameters() const
{
	return mHasParameters;
}

const std::vector<std::string>& DefineNode::GetParameters() const
{
	return mParameters;
}

IfNode::IfNode(std::unique_ptr<ExpressionNode> expression, std::unique_ptr<Node> body, std::unique_ptr<Node> elseBody) :
	mExpression(move(expression)),
	mBody(move(body)),
	mElseBody(move(elseBody))
{

}

void IfNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

ExpressionNode& IfNode::GetExpression()
{
	return *mExpression;
}

Node& IfNode::GetBody()
{
	return *mBody;
}

Node* IfNode::GetElse()
{
	return mElseBody.get();
}

IncludeNode::IncludeNode(const std::string& filename, IncludeType includeType) :
	mFilename(filename),
	mIncludeType(includeType)
{

}
	
void IncludeNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

const std::string& IncludeNode::GetFilename() const
{
	return mFilename;
}

IncludeNode::IncludeType IncludeNode::GetIncludeType() const
{
	return mIncludeType;
}

LineNode::LineNode(size_t line, const std::string& file)
{

}

void LineNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

PragmaNode::PragmaNode()
{

}

void PragmaNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

UnDefNode::UnDefNode(const std::string& name) :
	mName(name)
{

}

void UnDefNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

const std::string& UnDefNode::GetName() const
{
	return mName;
}

}