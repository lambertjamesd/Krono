#include "DirectiveNode.h"
#include "NodeVisitor.h"

namespace preproc
{

DefineNode::DefineNode(const Token& token, const std::string& name, const std::vector<std::string>& parameters, bool hasParameters, std::unique_ptr<Node> value) :
	DirectiveNode(token),
	mName(name),
	mParameters(parameters),
	mHasParameters(hasParameters),
	mValue(move(value))
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

bool DefineNode::HasParameters() const
{
	return mHasParameters;
}

const std::vector<std::string>& DefineNode::GetParameters() const
{
	return mParameters;
}

IfNode::IfNode(const Token& token, std::unique_ptr<Node> expression, std::unique_ptr<Node> body, std::unique_ptr<Node> elseBody) :
	DirectiveNode(token),
	mExpression(move(expression)),
	mBody(move(body)),
	mElseBody(move(elseBody))
{

}

void IfNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

Node& IfNode::GetExpression()
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

IncludeNode::IncludeNode(const Token& token, const std::string& filename, IncludeType includeType) :
	DirectiveNode(token),
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

LineNode::LineNode(const Token& token, size_t line, const std::string& file) :
	DirectiveNode(token)
{

}

void LineNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

PragmaNode::PragmaNode(const Token& token) :
	DirectiveNode(token)
{

}

void PragmaNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

UnDefNode::UnDefNode(const Token& token, const std::string& name) :
	DirectiveNode(token),
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
